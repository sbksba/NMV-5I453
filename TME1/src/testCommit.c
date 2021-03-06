#include<stdlib.h>
#include<stdio.h>

#include"commit.h"

int main(int argc, char const *argv[])
{
	struct commit *first = new_commit(0, 0, "First !");
	struct commit *tmp, *victim, *last;

	/*first->display = display_commit_major;*/
	list_add(&first->lhead, &list_complete);
	list_add(&first->major_list, &list_major);

	display_commit_major(first);
	printf("\n");

	display_history(first);

	tmp = add_minor_commit(first, "Work 1");
	tmp = add_minor_commit(tmp, "Work 2");
	victim = add_minor_commit(tmp, "Work 3");
	last = add_minor_commit(victim, "Work 4");
	display_history(first);

	del_commit(victim);
	display_history(first);

	tmp = add_major_commit(last, "Release 1");
	tmp = add_minor_commit(tmp, "Work 1");
	tmp = add_minor_commit(tmp, "Work 2");
	tmp = add_major_commit(tmp, "Release 2");
	tmp = add_minor_commit(tmp, "Work 1");
	display_history(first);

	add_minor_commit(last, "Oversight !!!");
	display_history(first);

	infos(first, 1, 2);

	infos(first, 1, 7);

	infos(first, 4, 2);

	#ifdef DEBUG
	printf("\n== TEST SUPP MAJEUR ==\n");
	del_commit(first);
	display_history(NULL);
	#endif

	freeHistory();

	return 0;
}
