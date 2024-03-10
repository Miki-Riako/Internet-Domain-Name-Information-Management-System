import os

def count_lines(folder):
    total = 0
    
    for root, dirs, files in os.walk(folder):
        for file in files:
            if file.endswith('.cpp') or file.endswith('.h'):
                path = os.path.join(root, file)
                
                with open(path, 'r', encoding='utf-8') as f:
                    lines = len([line for line in f])
                    total += lines
    
    return total

total_lines = count_lines('./src') 
print(total_lines)