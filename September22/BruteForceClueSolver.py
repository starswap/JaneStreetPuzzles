#Download an english dictionary to use and save it in dictionary.txt

def getCounts(word):
	counts = {}
	for c in word:
		if c in counts:
			counts[c] += 1
		else:
			counts[c] = 1
	return counts

needed = [(3,1,1),(3,1,3,4,2),(4,4,4,4),(6,6,7,2,4),(2,2,3,4,2),(5,3,1),(6,6,3,1)]

doubles = []
with open("dictionary.txt") as f:
	for line in f:
		word = line.strip().split(" – ")[0].lower()
		print(word)
		if (not(word.isalpha())):
			continue
		counts = getCounts(word)
		flag = True
		for key in counts:
			count = counts[key]
			flag = flag and (count == 2)
		if flag and len(word) >= 6 and len(word) <= 10:
			doubles.append(word)


matches = {}
for dbl in doubles:
	for need in needed:
		if (len(dbl) != len(need)*2):
			continue
		letters = set(dbl)
		idx = 0
		ok = True
		for i,char in enumerate(dbl):
			if char in letters:
				if (i+need[idx] < len(dbl) and dbl[i+need[idx]] == char):
					letters.remove(char)
					idx+= 1
				else:
					ok = False
		if ok:
			if str(need) in matches:
				matches[str(need)].append(dbl)
			else:
				matches[str(need)] = [dbl]

for match in matches:
	print("Clue " + match)
	print("Possibilities:")
	for poss in matches[match]:
		print(f"\t {poss}")
