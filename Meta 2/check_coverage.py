import os
import re

spec_nodes = [
    "Program", "FieldDecl", "VarDecl", "MethodDecl", "MethodHeader",
    "MethodParams", "ParamDecl", "MethodBody", "Block", "If", "While",
    "Return", "Call", "Print", "ParseArgs", "Assign", "Or", "And",
    "Eq", "Ne", "Lt", "Gt", "Le", "Ge", "Add", "Sub", "Mul", "Div",
    "Mod", "Lshift", "Rshift", "Xor", "Not", "Minus", "Plus", "Length",
    "Bool", "BoolLit", "Double", "Decimal", "Identifier", "Int",
    "Natural", "StrLit", "StringArray", "Void"
]

out_files = [f for f in os.listdir("meta2") if f.endswith(".myout")]
found_nodes = set()

for out_file in out_files:
    with open(os.path.join("meta2", out_file), 'r') as f:
        content = f.read()
        for node in spec_nodes:
            if re.search(r'\b' + node + r'\b', content):
                found_nodes.add(node)

missing_nodes = set(spec_nodes) - found_nodes
print("Missing nodes in tests:", sorted(list(missing_nodes)))

