#pragma once

/*
	Avoid early out/nested if statements with try/catch style logic.

	Example: Simple early out checks

		stry
		{
			scheckall(ptr1);
			scheckall(ptr2);
			scheckall(IsSignedIn());
		}
		scatchall
		{
		}
		sfinally
		{
		}

	Example: With cleanup needed after a certain point

	stry
	{
		scheckall(ptr1);
		scheckall(ptr2);

		Init();
		scheck(IsSignedIn(), cleanup)
	}
	scatch(cleanup)
	{
		Cleanup();
	}
	sfinally
	{

	}
*/

#define stry
#define scheck(cond, LABEL) do { if (!(cond)) {  goto LABEL; } } while(0);
#define scheckand(cond, LABEL, work) do { if (!(cond)) { work; goto LABEL; } } while(0);
#define scheckall(cond) do { if (!(cond)) { goto CATCHALL; } } while (0);
#define scatch(LABEL) LABEL:
#define sthrow(LABEL) goto LABEL;
#define scatchall if (false) CATCHALL:
#define sfinally