#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"comment.h"

struct comment *new_comment(
	int title_size, char *title,
	int author_size, char *author,
	int text_size, char *text)
{
	struct comment *c = (struct comment *) malloc(sizeof(struct comment));

	c->title_size = title_size;
	if (!(c->title = malloc(title_size)))
		goto badTitle;
	memcpy(c->title, title, title_size);

	c->author_size = author_size;
	if (!(c->author = malloc(author_size)))
		goto badAuthor;
	memcpy(c->author, author, author_size);

	c->text_size = text_size;
	if (!(c->text = malloc(text_size)))
		goto badText;
	memcpy(c->text, text, text_size);

	return c;

badText:
	free(c->text);
	printf("## BAD TEXT SIZE ##\n");
	return NULL;

badAuthor:
	free(c->author);
	printf("## BAD AUTHOR SIZE ##\n");
	return NULL;
	
badTitle:
	free(c->title);
	free(c);
	printf("## BAD TITLE ##\n");
	return NULL;
}

void display_comment(struct comment *c)
{
	printf("%s from %s \"%s\"\n", c->title, c->author, c->text);
}
