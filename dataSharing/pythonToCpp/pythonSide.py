def output_for_file(listy, cover):
    with open('outputFile.txt', 'w' if cover else 'a+') as fil:
        for i in listy:
            fil.write(str(i) + " ")
        fil.write('\n')