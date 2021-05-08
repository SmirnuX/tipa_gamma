all:
	gcc gamma.c string_parser.c -o tgamma
sanitized:
	gcc gamma.c string_parser.c -o tgamma -fsanitize=address