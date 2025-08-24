#include <unordered_map>

struct ModelTree {
    Entity3D* root = nullptr;
    std::string rootName; 
    std::unordered_map<std::string, Entity3D*> byName;
    std::unordered_map<std::string, Entity3D*> byPath; 
};

inline void IndexTree(Entity3D* node, ModelTree& out, std::string path = "")
{
    if (!node) return;

    if (path.empty()) {
        path = node->GetName();
        out.rootName = path;
    }

    out.byName[node->GetName()] = node;

    out.byPath[path] = node;

    const std::string& root = out.rootName;
    if (!root.empty() && path.rfind(root + "/", 0) == 0) {
        out.byPath[path.substr(root.size() + 1)] = node;
    }
    if (path == root) {
        out.byPath[""] = node;
        out.byPath[root] = node;
    }

    for (auto* ch : node->GetChildren()) {
        if (auto* e3d = dynamic_cast<Entity3D*>(ch)) {
            IndexTree(e3d, out, path + "/" + e3d->GetName());
        }
    }
}

inline Entity3D* FindByPath(const ModelTree& tree, const std::string& path) {
    auto it = tree.byPath.find(path);
    return it == tree.byPath.end() ? nullptr : it->second;
}

inline Entity3D* FindByName(const ModelTree& tree, const std::string& name) {
    auto it = tree.byName.find(name);
    return it == tree.byName.end() ? nullptr : it->second;
}
