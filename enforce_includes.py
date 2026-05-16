#!/usr/bin/env python3
"""
Pattern B include enforcer for Opto.

Rules enforced:
  1. Each .cpp must include its own .h
  2. Each .cpp must not explicitly re-include what its .h already includes directly
     (those arrive transitively — the explicit repeat is redundant noise)
  3. All quoted includes in both .h and .cpp must be in alphabetical order

Rule NOT enforced automatically (requires C++ parsing):
  4. Includes in .h that are only needed by the .cpp implementation should move to .cpp.
     Check manually: if removing an include from .h doesn't break callers of the header,
     it probably belongs in the .cpp instead.

Usage:
  python enforce_includes.py           # apply fixes
  python enforce_includes.py --dry-run # preview only, no file changes
"""

import re
import sys
from pathlib import Path

SRC = Path("src")
DRY_RUN = "--dry-run" in sys.argv


def read_includes(path: Path) -> list[str]:
    with open(path, encoding="utf-8") as f:
        return [m.group(1) for line in f if (m := re.match(r'\s*#include\s+"([^"]+)"', line))]


def write_lines_collapsed(path: Path, lines: list[str]):
    """Write lines, collapsing consecutive blank lines (used after removals)."""
    if DRY_RUN:
        return
    result = []
    prev_blank = False
    for line in lines:
        blank = line.strip() == ""
        if blank and prev_blank:
            continue
        result.append(line)
        prev_blank = blank
    with open(path, "w", encoding="utf-8") as f:
        f.writelines(result)


def remove_include(path: Path, target: str) -> bool:
    with open(path, encoding="utf-8") as f:
        lines = f.readlines()
    new_lines = []
    removed = False
    for line in lines:
        if not removed:
            m = re.match(r'\s*#include\s+"([^"]+)"', line)
            if m and Path(m.group(1)).name == target:
                removed = True
                continue
        new_lines.append(line)
    if removed:
        write_lines_collapsed(path, new_lines)
    return removed


def add_include_before_first(path: Path, target: str) -> bool:
    with open(path, encoding="utf-8") as f:
        content = f.read()
    if f'"{target}"' in content:
        return False
    line_to_add = f'#include "{target}"\n'
    first = re.search(r'^\s*#include\s+[<"]', content, re.MULTILINE)
    if first:
        pos = first.start()
        content = content[:pos] + line_to_add + content[pos:]
    else:
        content = line_to_add + "\n" + content
    if not DRY_RUN:
        with open(path, "w", encoding="utf-8") as f:
            f.write(content)
    return True


def sort_includes(path: Path) -> list[str] | None:
    """Sort quoted #include lines alphabetically in-place.
    Returns the sorted name list if a change was made, None otherwise."""
    with open(path, encoding="utf-8") as f:
        lines = f.readlines()

    # Collect (line_index, include_name, original_line) for all quoted includes
    entries = []
    for i, line in enumerate(lines):
        m = re.match(r'\s*#include\s+"([^"]+)"', line)
        if m:
            entries.append((i, m.group(1), line))

    if len(entries) <= 1:
        return None

    original_names = [name for _, name, _ in entries]
    sorted_entries = sorted(entries, key=lambda e: e[1].lower())
    sorted_names = [name for _, name, _ in sorted_entries]

    if original_names == sorted_names:
        return None

    new_lines = list(lines)
    for (idx, _, _), (_, _, sorted_line) in zip(entries, sorted_entries):
        new_lines[idx] = sorted_line

    if not DRY_RUN:
        with open(path, "w", encoding="utf-8") as f:
            f.writelines(new_lines)

    return sorted_names


def main():
    pairs: list[tuple[Path, Path]] = []
    for cpp in sorted(SRC.rglob("*.cpp")):
        h = cpp.with_suffix(".h")
        if h.exists():
            pairs.append((cpp, h))

    if not pairs:
        print("No .cpp/.h pairs found — run from the project root.")
        sys.exit(1)

    if DRY_RUN:
        print("DRY RUN — no files will be changed.\n")

    total_changed = 0

    for cpp, h in pairs:
        changes: list[str] = []

        h_includes = {Path(i).name for i in read_includes(h)}
        h_name = h.name

        # Rule 1: .cpp must include its own .h
        cpp_include_names = [Path(i).name for i in read_includes(cpp)]
        if h_name not in cpp_include_names:
            if add_include_before_first(cpp, h_name):
                changes.append(f"  + added   #include \"{h_name}\"")

        # Rule 2: .cpp must not re-include what .h already includes directly
        # Re-read after possible rule-1 modification
        for raw_inc in read_includes(cpp):
            inc_base = Path(raw_inc).name
            if inc_base == h_name:
                continue
            if inc_base in h_includes:
                if remove_include(cpp, inc_base):
                    changes.append(f"  - removed #include \"{raw_inc}\" (already in {h_name})")

        # Rule 3: alphabetical order in both files (re-read after any modifications above)
        sorted_h = sort_includes(h)
        if sorted_h:
            changes.append(f"  ~ sorted  {h_name}: {sorted_h}")

        sorted_cpp = sort_includes(cpp)
        if sorted_cpp:
            changes.append(f"  ~ sorted  {cpp.name}: {sorted_cpp}")

        label = f"{cpp.relative_to(SRC)}"
        if changes:
            total_changed += 1
            print(f"[CHANGED] {label}")
            for c in changes:
                print(c)
        else:
            print(f"[OK]      {label}")

    print(f"\n{'Would change' if DRY_RUN else 'Changed'} {total_changed} / {len(pairs)} file(s).")


if __name__ == "__main__":
    main()
