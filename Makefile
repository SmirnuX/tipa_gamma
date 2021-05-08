all:
	gcc src/gamma.c src/string_parser.c -o tgamma
sanitized:
	gcc src/gamma.c src/string_parser.c -o tgamma -fsanitize=address