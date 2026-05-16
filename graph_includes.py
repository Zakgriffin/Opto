#!/usr/bin/env python3
"""
Generates a Graphviz DOT graph of #include dependencies across src/ .h and .cpp files.
Paste the output into edotor.net to visualize.
"""

import os
import re
import hashlib
import colorsys
from pathlib import Path

SRC_ROOT = Path(__file__).parent / "src"

def dir_color(subdir: str) -> str:
    h = int(hashlib.md5(subdir.encode()).hexdigest()[:8], 16)
    hue = (h % 360) / 360.0
    r, g, b = colorsys.hls_to_rgb(hue, 0.88, 0.45)
    return "#{:02x}{:02x}{:02x}".format(int(r * 255), int(g * 255), int(b * 255))

def collect_files():
    """
    Returns (canonical_to_rels, fname_to_canonical).
    Each .cpp/.h pair in the same directory becomes one canonical node (the .cpp).
    Header-only files use the .h as canonical.
    fname_to_canonical maps any filename (foo.cpp or foo.h) -> canonical rel path.
    """
    from collections import defaultdict
    groups = defaultdict(dict)  # (dir, stem) -> {ext: rel}
    for root, _, filenames in os.walk(SRC_ROOT):
        for fname in filenames:
            if fname.endswith(".cpp"):
                stem, ext = fname[:-4], ".cpp"
            elif fname.endswith(".h"):
                stem, ext = fname[:-2], ".h"
            else:
                continue
            full = Path(root) / fname
            rel = full.relative_to(SRC_ROOT).as_posix()
            dir_part = rel.rsplit("/", 1)[0] if "/" in rel else ""
            groups[(dir_part, stem)][ext] = rel

    canonical_to_rels = {}   # canonical rel -> set of all rels in the pair
    fname_to_canonical = {}  # any filename -> canonical rel
    for (_, stem), exts in groups.items():
        canonical = exts.get(".cpp") or exts[".h"]
        all_rels = set(exts.values())
        canonical_to_rels[canonical] = all_rels
        for rel in all_rels:
            fname_to_canonical[rel.rsplit("/", 1)[-1]] = canonical
    return canonical_to_rels, fname_to_canonical

def parse_local_includes(filepath: Path):
    includes = []
    try:
        with open(filepath, encoding="utf-8", errors="ignore") as f:
            for line in f:
                m = re.match(r'\s*#include\s+"([^"]+)"', line)
                if m:
                    includes.append(m.group(1))
    except OSError:
        pass
    return includes

def node_id(name: str) -> str:
    return re.sub(r"[^a-zA-Z0-9_]", "_", name)

def main():
    canonical_to_rels, fname_to_canonical = collect_files()

    # Build edges between canonical nodes
    edges = set()
    for canonical, all_rels in canonical_to_rels.items():
        for rel in all_rels:
            for inc in parse_local_includes(SRC_ROOT / rel):
                inc_base = os.path.basename(inc)
                if inc_base in fname_to_canonical:
                    target = fname_to_canonical[inc_base]
                    if canonical != target:
                        edges.add((canonical, target))

    # Detect mutual (A↔B) cycles → red
    mutual = {(a, b) for (a, b) in edges if (b, a) in edges}

    # Detect longer cycles via Tarjan's SCC
    # Edges in a multi-node SCC but not a direct 2-cycle → orange
    graph = {}
    for canonical in canonical_to_rels:
        graph[canonical] = set()
    for (a, b) in edges:
        graph[a].add(b)

    index_counter = [0]
    stack = []
    lowlink = {}
    index = {}
    on_stack = {}
    node_to_scc = {}

    def strongconnect(v):
        index[v] = lowlink[v] = index_counter[0]
        index_counter[0] += 1
        stack.append(v)
        on_stack[v] = True
        for w in graph.get(v, ()):
            if w not in index:
                strongconnect(w)
                lowlink[v] = min(lowlink[v], lowlink[w])
            elif on_stack.get(w):
                lowlink[v] = min(lowlink[v], index[w])
        if lowlink[v] == index[v]:
            scc = []
            while True:
                w = stack.pop()
                on_stack[w] = False
                scc.append(w)
                if w == v:
                    break
            scc_id = frozenset(scc)
            for w in scc:
                node_to_scc[w] = scc_id

    import sys
    sys.setrecursionlimit(10000)
    for v in list(graph):
        if v not in index:
            strongconnect(v)

    in_longer_cycle = {
        (a, b) for (a, b) in edges
        if len(node_to_scc.get(a, frozenset())) > 1
        and node_to_scc.get(a) == node_to_scc.get(b)
        and (a, b) not in mutual
    }

    # Compute topological depth (longest path from any source) ignoring cyclic edges
    all_nodes = set(canonical_to_rels.keys())
    dag_edges = edges - mutual - in_longer_cycle
    out_adj = {n: set() for n in all_nodes}
    in_deg  = {n: 0     for n in all_nodes}
    for (a, b) in dag_edges:
        out_adj[a].add(b)
        in_deg[b] += 1

    from collections import deque
    depth = {n: 0 for n in all_nodes}
    queue = deque(n for n in all_nodes if in_deg[n] == 0)
    while queue:
        v = queue.popleft()
        for w in out_adj[v]:
            if depth[w] < depth[v] + 1:
                depth[w] = depth[v] + 1
            in_deg[w] -= 1
            if in_deg[w] == 0:
                queue.append(w)

    by_depth = {}
    for n, d in depth.items():
        by_depth.setdefault(d, []).append(n)

    lines = ["digraph includes {"]
    lines.append("    rankdir=TB;")
    lines.append("    newrank=true;")
    lines.append('    node [shape=box, style=filled, fontname="Helvetica", fontsize=10];')
    lines.append('    edge [fontsize=9];')
    lines.append("")

    # Rank groups — one per depth level
    for d in sorted(by_depth):
        ids = " ".join(node_id(n) for n in sorted(by_depth[d]))
        lines.append(f"    {{ rank=same; {ids} }}")
    lines.append("")

    # Node declarations grouped by subdir (clusters)
    by_dir = {}
    for canonical in sorted(canonical_to_rels):
        parts = canonical.split("/")
        subdir = parts[0] if len(parts) > 1 else "src"
        by_dir.setdefault(subdir, []).append(canonical)

    for subdir, canonicals in sorted(by_dir.items()):
        color = dir_color(subdir)
        lines.append(f"    subgraph cluster_{subdir} {{")
        lines.append(f'        label="{subdir}"; fontsize=12; fontname="Helvetica-Bold";')
        lines.append(f'        style=filled; fillcolor="{color}"; color="#888888";')
        for canonical in sorted(canonicals):
            label = canonical.rsplit("/", 1)[-1]
            nid = node_id(canonical)
            lines.append(f'        {nid} [label="{label}", fillcolor="{color}"];')
        lines.append("    }")
        lines.append("")

    VIEW_BUILDERS = fname_to_canonical.get("view_builders.cpp")
    OBJECT_VIEW   = fname_to_canonical.get("object_view.cpp")
    GLOBALS       = fname_to_canonical.get("globals.cpp")

    # Edges — priority: red > orange > blue > lightblue > pink > black
    for (a, b) in sorted(edges):
        if (a, b) in mutual:
            color = "red"
        elif (a, b) in in_longer_cycle:
            color = "orange"
        elif a == VIEW_BUILDERS:
            color = "blue"
        elif b == OBJECT_VIEW:
            color = "lightblue"
        elif b == GLOBALS:
            color = "pink"
        else:
            color = "black"
        lines.append(f'    {node_id(a)} -> {node_id(b)} [color="{color}"];')

    lines.append("}")

    dot = "\n".join(lines)
    print(dot)

    # Also write to file for convenience
    out = Path(__file__).parent / "includes_graph.dot"
    out.write_text(dot, encoding="utf-8")
    print(f"\n// Written to {out}", flush=True)

if __name__ == "__main__":
    main()
