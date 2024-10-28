import json
import networkx as nx
import matplotlib.pyplot as plt
from anytree import Node, RenderTree, LevelOrderIter

# Load JSON data from a file
def load_json(filename):
    with open(filename, 'r') as file:
        return json.load(file)

# Recursively create nodes for the tree structure
def create_tree(data, parent=None):
    if isinstance(data, dict):
        for key, value in data.items():
            node = Node(key, parent=parent)
            create_tree(value, parent=node)
    elif isinstance(data, list):
        for index, item in enumerate(data):
            node = Node(f"Item {index}", parent=parent)
            create_tree(item, parent=node)
    else:
        Node(str(data), parent=parent)

# Draw the tree graphically using NetworkX and Matplotlib
def draw_graph(root):
    G = nx.DiGraph()

    # Add edges to the graph
    for node in LevelOrderIter(root):
        if node.parent is not None:
            G.add_edge(node.parent.name, node.name)

    # Draw the graph
    pos = nx.spring_layout(G)
    plt.figure(figsize=(12, 8))
    nx.draw(G, pos, with_labels=True, node_size=3000, node_color="lightblue", font_size=10, font_weight='bold', edge_color="gray")
    plt.title("JSON Hierarchical Tree View")
    plt.show()

# Main function to generate tree view from JSON file and plot it
def json_to_tree_view(filename):
    data = load_json(filename)
    root = Node("Root")
    create_tree(data, parent=root)
    
    # Print textual tree view
    for pre, _, node in RenderTree(root):
        print(f"{pre}{node.name}")
    
    # Graphical tree view
    draw_graph(root)


# Example usage
json_to_tree_view('rules.json')
