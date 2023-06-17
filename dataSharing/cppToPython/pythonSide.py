def single_line():
    # 适用单行，返回列表[]
    with open('outputFile.txt', 'r') as fil:
        nums_str = fil.read().split()
        nums = [float(num) for num in nums_str]
    return nums


def multi_lines():
    # 适用多行，返回列表[[],[]]
    with open('outputFile.txt', 'r') as fil:
        lines = [line.strip().split() for line in fil]
        lines_s = [[float(num) for num in line] for line in lines]
    lines_s_filtered = [iList for iList in lines_s if len(iList) > 0]
    return lines_s_filtered
