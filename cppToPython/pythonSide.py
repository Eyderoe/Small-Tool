def single_line():
    # 适用单行
    with open('outputFile.txt', 'r') as fil:
        nums_str = fil.read().split()
        nums = [float(num) for num in nums_str]
    return nums


def multi_lines():
    # 适用多行
    with open('outputFile.txt', 'r') as fil:
        lines = [line.strip().split() for line in fil]
        lines_s = [[float(num) for num in line] for line in lines]
    return lines_s
