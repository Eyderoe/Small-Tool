# 适用单行
with open('outputFile.txt', 'r') as fil:
    nums_str = fil.read().split()
    nums = [float(num) for num in nums_str]

# 适用多行
with open('outputFile.txt', 'r') as fil:
    lines = [line.strip().split() for line in fil]
    lines_s = [[float(num) for num in line] for line in lines]

print(nums)
print(lines_s)
