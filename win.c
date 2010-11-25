#include <curses.h>
#include <stdlib.h>

#define ENTER 10
#define ESCAPE 27
void init_curses()
{
        initscr();
        start_color();
        init_pair(1,COLOR_WHITE,COLOR_BLUE);
        init_pair(2,COLOR_BLUE,COLOR_WHITE);
        init_pair(3,COLOR_RED,COLOR_WHITE);
        curs_set(0);
        noecho();
        keypad(stdscr,TRUE);
}
void draw_menubar(WINDOW *menubar)
{
        wbkgd(menubar,COLOR_PAIR(2));
        waddstr(menubar,"Menu1");
        wattron(menubar,COLOR_PAIR(3));
        waddstr(menubar,"(F1)");
        wattroff(menubar,COLOR_PAIR(3));
        wmove(menubar,0,20);
        waddstr(menubar,"Menu2");
        wattron(menubar,COLOR_PAIR(3));
        waddstr(menubar,"(F2)");
        wattroff(menubar,COLOR_PAIR(3));
}
WINDOW **draw_menu(int start_col)
{
        int i;
        WINDOW **items;
        items=(WINDOW **)malloc(9*sizeof(WINDOW *));

        items[0]=newwin(10,19,1,start_col);
        wbkgd(items[0],COLOR_PAIR(2));
        box(items[0],ACS_VLINE,ACS_HLINE);
        items[1]=subwin(items[0],1,17,2,start_col+1);
        items[2]=subwin(items[0],1,17,3,start_col+1);
        items[3]=subwin(items[0],1,17,4,start_col+1);
        items[4]=subwin(items[0],1,17,5,start_col+1);
        items[5]=subwin(items[0],1,17,6,start_col+1);
        items[6]=subwin(items[0],1,17,7,start_col+1);
        items[7]=subwin(items[0],1,17,8,start_col+1);
        items[8]=subwin(items[0],1,17,9,start_col+1);
        for (i=1;i<9;i++)
	  wprintw(items[i],"Item%d",i);
        wbkgd(items[1],COLOR_PAIR(1));
        wrefresh(items[0]);
        return items;
}
void delete_menu(WINDOW **items,int count)
{
        int i;
        for (i=0;i<count;i++)
	  delwin(items[i]);
        free(items);
}
int scroll_menu(WINDOW **items,int count,int menu_start_col)
{
        int key;
        int selected=0;
        while (1) {
                key=getch();
                if (key==KEY_DOWN || key==KEY_UP) {
                        wbkgd(items[selected+1],COLOR_PAIR(2));
                        wnoutrefresh(items[selected+1]);
                        if (key==KEY_DOWN) {
                                selected=(selected+1) % count;
                        } else {
                                selected=(selected+count-1) % count;
                        }
                        wbkgd(items[selected+1],COLOR_PAIR(1));
                        wnoutrefresh(items[selected+1]);
                        doupdate();
                } else if (key==KEY_LEFT || key==KEY_RIGHT) {
                        delete_menu(items,count+1);
                        touchwin(stdscr);
                        refresh();
                        items=draw_menu(20-menu_start_col);
                        return scroll_menu(items,8,20-menu_start_col);
                } else if (key==ESCAPE) {
                        return -1;
                } else if (key==ENTER) {
                        return selected;
                }
        }
}
int main()
{
	int key;
	WINDOW *menubar,*messagebar;
    
	init_curses();
    
	bkgd(COLOR_PAIR(1));
	menubar=subwin(stdscr,1,80,0,0);
	messagebar=subwin(stdscr,1,79,23,1);
	draw_menubar(menubar);
	move(2,1);
	printw("Press F1 or F2 to open the menus. ");
	printw("ESC quits.");
	refresh();

	do {
		int selected_item;
		WINDOW **menu_items;
		key=getch();
		werase(messagebar);
		wrefresh(messagebar);
		if (key==KEY_F(1)) {
			menu_items=draw_menu(0);
			selected_item=scroll_menu(menu_items,8,0);
			delete_menu(menu_items,9);
			if (selected_item<0)
			  wprintw(messagebar,"You haven't selected any item.");
			else
			  wprintw(messagebar,
				  "You have selected menu item %d.",selected_item+1);
			touchwin(stdscr);
			refresh();
		} else if (key==KEY_F(2)) {
			menu_items=draw_menu(20);
			selected_item=scroll_menu(menu_items,8,20);
			delete_menu(menu_items,9);
			if (selected_item<0)
			  wprintw(messagebar,"You haven't selected any item.");
			else
			  wprintw(messagebar,
				  "You have selected menu item %d.",selected_item+1);
			touchwin(stdscr);
			refresh();
		}
	} while (key!=ESCAPE);
    
	delwin(menubar);
	delwin(messagebar);
	endwin();
	return 0;
}

