#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct Node {
    string name;
    bool is_directory;
    long size;
    Node* parent;

    vector<Node*> children;
};

void insert_node(Node* root, string name, long size, bool is_directory, Node* parent) {
    Node* node = new Node;
    node->name = name;
    node->size = size;
    node->is_directory = is_directory;
    node->parent = parent;

    if (parent == nullptr) {
        root->children.push_back(node);
    }
    else {
        parent->children.push_back(node);
    }
}

void parse_input_file(Node* root, string input_file) {
    ifstream input(input_file);

    Node* cwd = root;

    string line;
    while (getline(input, line)) {
        if (line.at(0) == '$') {
            if (line.find("cd") != string::npos) {
                string name = line.substr(line.find("cd ") + 3);
                if (name == "..") {
                    cwd = cwd->parent;
                    continue;
                }
                else if (name == "/") {
                    cwd = root;
                    continue;
                }
                bool found = false;

                for (Node* child : cwd->children) {
                    if (child->name == name && child->is_directory) {
                        cwd = child;
                        found = true;
                        break;
                    }
                }
            }
            else {
                // ls command, ignore
            }
        }
        else {
            int space_index = line.find(" ");
            string arg1 = line.substr(0, space_index);
            string name = line.substr(space_index + 1);
            long size = 0;

            bool is_directory = false;
            if (arg1.find("dir") != string::npos) {
                is_directory = true;
            }
            else {
                size = stol(arg1);
            }

            insert_node(root, name, size, is_directory, cwd);
        }
    }
}

void print_tree(Node* root, int indent) {
    if (root->is_directory) {
        cout << string(indent, ' ') << "\\" << root->name << endl;
    }
    else {
        cout << string(indent, ' ') << root->name << " (size " << root->size << ')' << endl;
    }

    for (Node* child : root->children) {
        print_tree(child, indent + 2);
    }
}

long calc_total_size(Node* node) {
    long total_size = 0;

    if (node->is_directory) {
        for (Node* child : node->children) {
            total_size += calc_total_size(child);
        }
    }
    else {
        total_size = node->size;
    }

    return total_size;
}

long part_one(Node* node, long* total_sum) {
    long total_size = 0;

    if (node->is_directory) {
        for (Node* child : node->children) {
            total_size += part_one(child, total_sum);
        }

        if (total_size <= 100000) {
            *total_sum += total_size;
        }
    }
    else {
        total_size = node->size;
    }

    return total_size;
}

long part_two(Node* node, vector<long>& sizes, long min_size) {
    long total_size = 0;

    if (node->is_directory) {
        for (Node* child : node->children) {
            total_size += part_two(child, sizes, min_size);
        }

        if (total_size >= min_size) {
            sizes.push_back(total_size);
        }
    }
    else {
        total_size = node->size;
    }

    return total_size;
}

int main() {
    Node root;
    root.name = "/";
    root.is_directory = true;
    root.size = 0;
    root.parent = nullptr;
    parse_input_file(&root, "input7.txt");
    long total_sum = 0;
    part_one(&root, &total_sum);
    cout << total_sum << endl;

    vector<long> dir_sizes;
    long min_size_required = calc_total_size(&root) - 40000000;
    part_two(&root, dir_sizes, min_size_required);
    sort(dir_sizes.begin(), dir_sizes.end());
    cout << dir_sizes.front() << endl;
}