#include "serialize.h"

template <typename T>
void write_raw(std::vector<char>& buffer, T value) {
    const char* ptr = reinterpret_cast<const char*>(&value);
    cout << sizeof(T);
    buffer.insert(buffer.end(), ptr, ptr + sizeof(T));
}

void serialize_ast(void* node, std::vector<char>& buffer) {
    if (!node) {
        write_raw(buffer, NONE);
        return;
    }

    ObjectType type = object_to_type.at(node);
    write_raw(buffer, static_cast<uint16_t>(type));

    switch (type) {
        case INTEGER: {
            write_raw(buffer, *(int*)node);
            break;
        }
        case DECLARE: {
            // Assuming Declare structures are identified by name/ID elsewhere,
            // or we serialize the name if available in object_to_name.
            if (object_to_name.contains(node)) {
                std::string name = object_to_name.at(node);
                int len = name.length();
                write_raw(buffer, len);
                buffer.insert(buffer.end(), name.begin(), name.end());
            }
            break;
        }
        case ADD: {
            auto* add = (Add*)node;
            serialize_ast(add->augend, buffer);
            serialize_ast(add->addend, buffer);
            break;
        }
        case SUB: {
            auto* sub = (Sub*)node;
            serialize_ast(sub->minuend, buffer);
            serialize_ast(sub->subtrahend, buffer);
            break;
        }
        case AND: {
            auto* and_obj = (And*)node;
            serialize_ast(and_obj->conjunct_1, buffer);
            serialize_ast(and_obj->conjunct_2, buffer);
            break;
        }
        case GREATER_THAN: {
            auto* gt = (GreaterThan*)node;
            serialize_ast(gt->left, buffer);
            serialize_ast(gt->right, buffer);
            break;
        }
        case GREATER_THAN_OR_EQUAL: {
            auto* gte = (GreaterThanOrEqual*)node;
            serialize_ast(gte->left, buffer);
            serialize_ast(gte->right, buffer);
            break;
        }
        case LESS_THAN: {
            auto* lt = (LessThan*)node;
            serialize_ast(lt->left, buffer);
            serialize_ast(lt->right, buffer);
            break;
        }
        case INDEX: {
            auto* idx = (Index*)node;
            serialize_ast(idx->array, buffer);
            serialize_ast(idx->at, buffer);
            break;
        }
        case VECTOR: {
            auto* v = (std::vector<void*>*)node;
            int size = static_cast<int>(v->size());
            write_raw(buffer, size);
            for (void* element : *v) {
                serialize_ast(element, buffer);
            }
            break;
        }
        case ASSIGN: {
            auto* assign = (Assign*)node;
            serialize_ast(assign->grantee, buffer);
            serialize_ast(assign->grantor, buffer);
            break;
        }
        case IF: {
            auto* cf = (If*)node;
            serialize_ast(cf->condition, buffer);
            serialize_ast(cf->then, buffer);
            serialize_ast(cf->finally, buffer);
            break;
        }
        case WHILE: {
            auto* cf = (While*)node;
            serialize_ast(cf->condition, buffer);
            serialize_ast(cf->then, buffer);
            serialize_ast(cf->finally, buffer);
            break;
        }
        case REPEAT: {
            auto* cf = (Repeat*)node;
            serialize_ast(cf->condition, buffer);
            serialize_ast(cf->then, buffer);
            serialize_ast(cf->finally, buffer);
            break;
        }
        case LOOP: {
            auto* cf = (Loop*)node;
            serialize_ast(cf->body, buffer);
            break;
        }
        case PROCEDURE: {
            auto* proc = (Procedure*)node;
            serialize_ast(proc->param, buffer);
            serialize_ast(proc->body, buffer);
            break;
        }
        case CALL: {
            auto* call = (Call*)node;
            serialize_ast(call->procedure, buffer);
            serialize_ast(call->param, buffer);
            break;
        }
        case DO_THEN: {
            auto* dt = (DoThen*)node;
            serialize_ast(dt->effect, buffer);
            serialize_ast(dt->next, buffer);
            break;
        }
        default:
            cout << "bad serialize " << type << endl;
            break;
    }
}

template <typename T>
T read_raw(const char*& ptr) {
    T value;
    std::memcpy(&value, ptr, sizeof(T));
    ptr += sizeof(T);
    return value;
}

std::string read_string(const char*& ptr) {
    int len = read_raw<int>(ptr);
    std::string str(ptr, len);
    ptr += len;
    return str;
}

void* deserialize_ast(const char*& ptr) {
    // 1. Read the type tag
    ObjectType type = read_raw<ObjectType>(ptr);

    if (type == NONE) {
        return nullptr;
    }

    void* node = nullptr;

    switch (type) {
        case INTEGER: {
            int* val = new int(read_raw<int>(ptr));
            node = val;
            break;
        }
        case DECLARE: {
            Declare* decl = new Declare();
            std::string name = read_string(ptr);
            object_to_name[decl] = name; // Restore name mapping
            node = decl;
            break;
        }
        case ADD: {
            Add* add = new Add();
            add->augend = deserialize_ast(ptr);
            add->addend = deserialize_ast(ptr);
            node = add;
            break;
        }
        case SUB: {
            Sub* sub = new Sub();
            sub->minuend = deserialize_ast(ptr);
            sub->subtrahend = deserialize_ast(ptr);
            node = sub;
            break;
        }
        case MUL: {
            Mul* mul = new Mul();
            mul->multiplicand = deserialize_ast(ptr);
            mul->multiplier = deserialize_ast(ptr);
            node = mul;
            break;
        }
        case AND: {
            And* and_obj = new And();
            and_obj->conjunct_1 = deserialize_ast(ptr);
            and_obj->conjunct_2 = deserialize_ast(ptr);
            node = and_obj;
            break;
        }
        case INDEX: {
            Index* idx = new Index();
            idx->array = deserialize_ast(ptr);
            idx->at = deserialize_ast(ptr);
            node = idx;
            break;
        }
        case VECTOR: {
            auto* v = new std::vector<void*>();
            int size = read_raw<int>(ptr);
            for (int i = 0; i < size; i++) {
                v->push_back(deserialize_ast(ptr));
            }
            node = v;
            break;
        }
        case ASSIGN: {
            Assign* assign = new Assign();
            assign->grantee = deserialize_ast(ptr);
            assign->grantor = deserialize_ast(ptr);
            node = assign;
            break;
        }
        case PROCEDURE: {
            Procedure* proc = new Procedure();
            proc->param = deserialize_ast(ptr);
            proc->body = deserialize_ast(ptr);
            node = proc;
            break;
        }
        default: {
            cout << "bad deserialize " << type << endl;
        }
    }

    // CRITICAL: Re-register the node type so the compiler knows what this is
    if (node) {
        object_to_type[node] = type;
    }

    return node;
}