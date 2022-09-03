#Finds PULLUP as for some reason it wasn't in the dictionary

vowels = "AEIOU"
consonants = "BCDFGHJKLMNQRSTVWXYZ"

for v in vowels:
	for c in consonants:
		print(f"P{v}{c}{c}{v}P")
