with open(r"C:\Users\marks\Desktop\pickles.txt",encoding="utf-8") as f:
    lines = map(lambda stri : stri.strip().split(" - ")[0],f.readlines())
    onlyEights = filter(lambda s : len(s) == 8, lines)
    print(list(onlyEights))
    print('\n'.join(list(onlyEights)))
