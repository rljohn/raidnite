#pragma once

/*
	Avoid early out/nested if statements with try/catch style logic.

	stry
	{
		scheck(ptr1);
		scheck(ptr2);
		scheck(IsSignedIn());
	}
	scatch(cleanup)
	{
		DoCleanup();
	}
	sfinally
	{
		
	}
*/

#define stry
#define scheck(cond, LABEL) do { if (!(cond)) {  goto LABEL; } } while(0);
#define scheckand(cond, LABEL, work) do { if (!(cond)) { work; goto LABEL; } } while(0);
#define scheckall(cond) do { if (!(cond)) { goto FINALLY; } } while (0);
#define scatch(LABEL) LABEL:
#define sthrow(LABEL) goto LABEL;
#define sthrowfinally() goto FINALLY;
#define sfinally FINALLY: