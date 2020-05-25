#include<gtk/gtk.h>
#include<cairo.h>
#include<math.h>
#include<ctype.h>
#include<string.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
typedef struct box {
	struct box **b;	
	GtkWidget *canvas;
	GtkWidget *eventbox;
	gint x, y;
	gint player;
	gint ballcount;
	gint cmass;
}box;
typedef struct box_n_count {
	gint width;
	gint height;
}box_n_count;
typedef struct gameplay_widgets {
	GtkWidget *pause[2];
	GtkWidget *save[2];
	GtkWidget *quit[2];
	GtkWidget *new[2];
}gameplay_widgets;
typedef struct table_container {
	GtkWidget *vbox4;
	GtkWidget *hbox4;
	gameplay_widgets widgets;
	GtkWidget *table;
	gint childs;
	box **b;
}table_container;
typedef struct help_page {
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *back;
	GtkWidget *textview;
	GtkTextBuffer *buffer;
	gchar *text;
}help_page;
typedef struct containers {
	help_page *help;
	GtkWidget **window;
	GtkWidget **notebook;
	GtkWidget **vbox1;
	GtkWidget **vbox2;
	GtkWidget **vbox3;	
	table_container *table;
	box ***b; //for accessing the pointer to each box in main function
}containers;
typedef struct p1widgets {
	GtkWidget *p1align[5];
	GtkWidget *p1widgets[5];
	GtkWidget *p1i[4];
}p1widgets;
typedef struct p2widgets {
	GtkWidget *p2align[3];
	GtkWidget *p2widgets[3];
	GtkWidget *p2i[3];
}p2widgets;
typedef struct p3widgets {
	GtkWidget *p3align[5];
	GtkWidget *p3widgets[5];
	GtkWidget *p3i[4];
}p3widgets;
typedef struct RGBA {
	gdouble r, g, b, a;
}RGBA;
gint NUM = 0;
gint FIRST = 1;
gint TWO = 2;
gint THREE = 3;
gint FOUR = 4;
gint FIVE = 5;
gint FLAG = 0;
gint HELP_FLAG = 0;
gint PAUSE = 0;
gint SAVE = 0;
gint T_PLAYERS = 0;
RGBA COLORS[6];
gint REMOVE[5] = {0, 0, 0, 0, 0};
gint BOX_COUNT[6] = { 0, 0, 0, 0, 0, 0};
gint CURR_COUNT;
containers *CONTAINER;
gint CH = 0;
//all function declaration
static void switch_page(GtkButton *button, GtkNotebook *notebook);
static void destroy(GtkWidget *window, table_container *table);
void free_table(table_container *table);
void set_playerball_colors();
box **make_board(table_container *table, gint x, containers *container);
void set_player_n_add_ball(box *b);
void explode(box **b, gint x, gint y, gint player);
void draw_line(GtkWidget *widget, GdkEvent *event, GtkWidget *canvas);
void draw_ball(box *b, gint player);
void draw_1_ball(GtkWidget *canvas, gint player);
void draw_2_ball(GtkWidget *canvas, gint player);
void draw_3_ball(GtkWidget *canvas, gint player);
void check_n_add(GtkWidget *widget, GdkEvent *event, box *b);
void ball_colors();
void quit_game(GtkWidget *button, containers *window);
void pause_game(GtkWidget *button, containers *container);
void help_pagef(GtkWidget *button, containers *container);
void free_help(help_page *help);
void go_first_page(GtkWidget *button, GtkNotebook *notebook);
void set_text(GtkWidget *window, GdkEvent *event, p1widgets *widgets);
void play_last_game(GtkWidget *button, containers *container);
void add_page1(containers *container, p1widgets *widgets);
void check(GtkWidget *entry, containers *container);
void back_page(GtkWidget *widget, GtkNotebook *notebook);
void add_page2(containers *container, p2widgets *widgets);
void select_players(GtkWidget *button, gint *i);
void add_page3(containers *container, p3widgets *widgets);
void pause_game(GtkWidget *button, containers *container);
void save_game(GtkWidget *button, box **b);
void new_game(GtkWidget *button, containers *container);
void remove_player(box **b, int *rm);
int check_all(box **b);
//all functions definitions
void pack_all_containers(containers *container, GtkWidget **window, GtkWidget **notebook, GtkWidget **vbox1, GtkWidget **vbox2, GtkWidget **vbox3, table_container *table, box ***b) {
	container->help = NULL;
	container->window = window;
	container->notebook = notebook;
	container->vbox1 = vbox1;
	container->vbox2 = vbox2;
	container->vbox3 = vbox3;
	container->table = table;
	container->b = b;
}
void add_page1(containers *container, p1widgets *widgets) {
	*(container->vbox1) = gtk_vbox_new(TRUE, 5);
	(container->help) = (help_page*)g_slice_alloc(sizeof(help_page));
	widgets->p1align[0] = gtk_alignment_new(0.5, 0, 1, 1);
	widgets->p1align[1] = gtk_alignment_new(0.5, 0.8, 0.4, 0.3);
	widgets->p1align[2] = gtk_alignment_new(0.5, 0.8, 0.4, 0.3);	
	widgets->p1align[3] = gtk_alignment_new(0.5, 0.8, 0.4, 0.3);
	widgets->p1align[4] = gtk_alignment_new(0.5, 0.8, 0.4, 0.3);
	widgets->p1widgets[0] = gtk_drawing_area_new();
	widgets->p1widgets[1] = gtk_button_new_with_label("START");
	widgets->p1widgets[2] = gtk_button_new_with_label("HOW");
	widgets->p1widgets[3] = gtk_button_new_with_label("LAST GAME");
	widgets->p1widgets[4] = gtk_button_new_with_label("QUIT");
	gtk_container_add(GTK_CONTAINER(widgets->p1align[0]), widgets->p1widgets[0]);
	gtk_container_add(GTK_CONTAINER(widgets->p1align[1]), widgets->p1widgets[1]);
	gtk_container_add(GTK_CONTAINER(widgets->p1align[2]), widgets->p1widgets[2]);
	gtk_container_add(GTK_CONTAINER(widgets->p1align[3]), widgets->p1widgets[3]);
	gtk_container_add(GTK_CONTAINER(widgets->p1align[4]), widgets->p1widgets[4]);
	gtk_box_pack_start(GTK_BOX(*(container->vbox1)), widgets->p1align[0], TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(*(container->vbox1)), widgets->p1align[1], TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(*(container->vbox1)), widgets->p1align[2], TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(*(container->vbox1)), widgets->p1align[3], TRUE, TRUE, 0);	
	gtk_box_pack_start(GTK_BOX(*(container->vbox1)), widgets->p1align[4], TRUE, TRUE, 0);
	//all signal connect
	g_signal_connect(G_OBJECT(*(container->window)), "expose-event", G_CALLBACK(set_text), widgets);
	g_signal_connect(G_OBJECT(widgets->p1widgets[1]), "clicked", G_CALLBACK(switch_page), (gpointer)(*(container->notebook)));
	g_signal_connect(G_OBJECT(widgets->p1widgets[2]), "clicked", G_CALLBACK(help_pagef), container);
	g_signal_connect(G_OBJECT(widgets->p1widgets[3]), "clicked", G_CALLBACK(play_last_game), (gpointer)(container));
	g_signal_connect(G_OBJECT(widgets->p1widgets[4]), "clicked", G_CALLBACK(quit_game), container);
	gtk_notebook_append_page(GTK_NOTEBOOK((*(container->notebook))), (*(container->vbox1)), NULL);
}
void play_last_game(GtkWidget *button, containers *container) {
	SAVE = 1;
	int fd, data;
	fd = open("save", O_RDONLY, S_IRUSR);
	read(fd, &data, sizeof(int));
	read(fd, &data, sizeof(int));
	NUM = data;
	close(fd);
	(*(container->b)) = make_board((container->table), NUM, container);
	if((gtk_notebook_insert_page(GTK_NOTEBOOK((*(container->notebook))), container->table->vbox4, NULL, -1)) == -1)
		printf("failed\n");
	gtk_widget_show_all((*(container->notebook)));
	gint n = gtk_notebook_get_n_pages(GTK_NOTEBOOK((*(container->notebook))));
	if(n == 4)
		gtk_notebook_set_current_page(GTK_NOTEBOOK((*(container->notebook))), 3);
	else
		gtk_notebook_set_current_page(GTK_NOTEBOOK((*(container->notebook))), 4);
}
void draw_balls(GtkWidget *notebook, GdkEvent *event, containers *container) {
	if(SAVE) {
		gint i, j;
		for(i = 0; i < container->table->childs; i++) {
			for(j = 0; j <container->table->childs; j++) {
				if((*(container->b))[i][j].ballcount == 1)
					draw_1_ball((*(container->b))[i][j].canvas, (*(container->b))[i][j].player);
				if((*(container->b))[i][j].ballcount == 2)
					draw_2_ball((*(container->b))[i][j].canvas, (*(container->b))[i][j].player);
				if((*(container->b))[i][j].ballcount == 3)
					draw_3_ball((*(container->b))[i][j].canvas, (*(container->b))[i][j].player);
			}
		}
		gtk_widget_queue_draw(container->table->widgets.pause[0]);
		gtk_widget_queue_draw(container->table->widgets.save[0]);
		gtk_widget_queue_draw(container->table->widgets.new[0]);
		gtk_widget_queue_draw(container->table->widgets.quit[0]);
	}
	SAVE = 0;
}
void set_text(GtkWidget *window, GdkEvent *event, p1widgets *widgets) {
	cairo_t *cr;
	cr = gdk_cairo_create(widgets->p1widgets[0]->window);
	cairo_text_extents_t extents;
	gint w, h;
	w = widgets->p1widgets[0]->allocation.width;
	h = widgets->p1widgets[0]->allocation.height;	
	cairo_select_font_face(cr, "Courier", CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, 50);
	cairo_text_extents(cr, "Chain Reaction", &extents);
	cairo_move_to(cr, w / 2 - extents.width / 2, h / 2);  
	cairo_show_text(cr, "Chain Reaction");
	cairo_text_extents(cr, "Game", &extents);
	cairo_move_to(cr, w / 2 - extents.width / 2, h / 1.5 + extents.height + 4);
	cairo_select_font_face(cr, "Courier", CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, 50);
	cairo_show_text(cr, "Game");
	cairo_destroy(cr);
}
void help_pagef(GtkWidget *button, containers *container) {
	if(!HELP_FLAG) {
		int fd = open("help.txt", O_RDONLY , S_IRUSR);
		gchar text[1024];
		gint i, iprev;
		while((i = read(fd, text, sizeof(text))))
			iprev = i;
		text[iprev] = '\0';
		close(fd);
		(container->help->text) = (char*)g_slice_alloc(sizeof(char) * (strlen(text) + 1));
		strcpy(container->help->text, text);
		container->help->vbox = gtk_vbox_new(FALSE, 2);
		container->help->hbox = gtk_hbox_new(FALSE, 1);
		container->help->back = gtk_button_new_with_label("BACK");
		g_signal_connect(G_OBJECT(container->help->back), "clicked", G_CALLBACK(go_first_page), (gpointer)(*(container->notebook)));
		container->help->textview = gtk_text_view_new();
		container->help->buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(container->help->textview));
		gtk_text_buffer_set_text(container->help->buffer, container->help->text, -1);
		gtk_box_pack_end(GTK_BOX(container->help->hbox), (container->help->back), FALSE, FALSE, 0);
		gtk_box_pack_start(GTK_BOX(container->help->vbox), (container->help->hbox), FALSE, FALSE, 0);
		gtk_box_pack_start(GTK_BOX(container->help->vbox), (container->help->textview), FALSE, FALSE, 0);	
		gtk_notebook_insert_page(GTK_NOTEBOOK(*(container->notebook)), (container->help->vbox), NULL, -1);	
		gtk_widget_show_all((*(container->notebook)));
		gtk_notebook_set_current_page(GTK_NOTEBOOK(*(container->notebook)), 3);
	}
	else {
		gtk_notebook_set_current_page(GTK_NOTEBOOK(*(container->notebook)), 3);
	}
	HELP_FLAG = 1;
}
void go_first_page(GtkWidget *button, GtkNotebook *notebook) {
	gtk_notebook_set_current_page(notebook, 0);
}
void free_help(help_page *help) {
	if(HELP_FLAG) {
		g_slice_free1(sizeof(char) * (strlen(help->text) + 1), help->text);
		g_slice_free1(sizeof(help_page), help);
	}
	HELP_FLAG = 0;
}
void quit_game(GtkWidget *button, containers *container) {
	GtkWidget *dialog, *label, *image, *hbox;
	dialog = gtk_dialog_new_with_buttons("QUIT", GTK_WINDOW(*(container->window)), GTK_DIALOG_MODAL, "YES", GTK_RESPONSE_YES, "NO", GTK_RESPONSE_NO, NULL);
	gtk_dialog_set_has_separator (GTK_DIALOG (dialog), FALSE);
	label = gtk_label_new ("ARE YOU WANT TO QUIT!");
	image = gtk_image_new_from_stock (GTK_STOCK_DIALOG_INFO,
	GTK_ICON_SIZE_DIALOG);
	hbox = gtk_hbox_new (FALSE, 5);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 10);
	gtk_box_pack_start_defaults (GTK_BOX (hbox), image);
	gtk_box_pack_start_defaults (GTK_BOX (hbox), label);
	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), hbox);
	gtk_widget_show_all (dialog);
	gint RESPONSE = gtk_dialog_run(GTK_DIALOG (dialog));
	if(RESPONSE == GTK_RESPONSE_NO) {
		gtk_widget_destroy (dialog);
		if(PAUSE)
			pause_game(NULL, container);
		return;
	}
	gtk_widget_destroy(dialog);
	free_help(container->help);
	destroy(*(container->window), container->table);
}
void add_page2(containers *container, p2widgets *widgets) {
	(*(container->vbox2)) = gtk_vbox_new(TRUE, 3);
	widgets->p2widgets[0] = gtk_button_new_with_label("SELECT BOARD");
	widgets->p2widgets[1] = gtk_entry_new();	//entry for board dimension	
	widgets->p2widgets[2] = gtk_button_new_with_label("BACK");
	widgets->p2align[0] = gtk_alignment_new(0.5, 0.7, 0.6, 0.55);
	widgets->p2align[1] = gtk_alignment_new(0.5, 0.7, 0.25, 0.4);
	widgets->p2align[2] = gtk_alignment_new(0.5, 0.7, 0.6, 0.55);
	gtk_container_add(GTK_CONTAINER(widgets->p2align[0]), widgets->p2widgets[0]);
	gtk_container_add(GTK_CONTAINER(widgets->p2align[1]), widgets->p2widgets[1]);
	gtk_container_add(GTK_CONTAINER(widgets->p2align[2]), widgets->p2widgets[2]);
	gtk_box_pack_start(GTK_BOX(*(container->vbox2)), widgets->p2align[0], TRUE, TRUE, 10);
	gtk_box_pack_start(GTK_BOX(*(container->vbox2)), widgets->p2align[1], TRUE, TRUE, 10);
	gtk_box_pack_start(GTK_BOX(*(container->vbox2)), widgets->p2align[2], TRUE, TRUE, 10);
	g_signal_connect(G_OBJECT(widgets->p2widgets[1]), "activate", G_CALLBACK(check), container);	
	g_signal_connect(G_OBJECT(widgets->p2widgets[2]), "clicked", G_CALLBACK(back_page), (gpointer)(*(container->notebook)));
	gtk_notebook_append_page(GTK_NOTEBOOK((*(container->notebook))), *(container->vbox2), NULL);
}
void back_page(GtkWidget *widget, GtkNotebook *notebook) {
	gint i = gtk_notebook_get_current_page(notebook);
	gtk_notebook_set_current_page(notebook, i - 1);
}
int ati(char *str) {
	int num = 0;
	while(*str != '\0') {
		num = num * 10 + ((*str) - '0');
		str++;
	}
	return num;
}
void check(GtkWidget *entry, containers *container) {
	const gchar *str, *address;
	gint i;
	str = gtk_entry_get_text(GTK_ENTRY(entry));
	address = str;	
	while((*str) != '\0') {
		if(isdigit(*str))
			i = 1;
		else {
			i = 0;
			break;
		}
		str++;
	}
	if(i == 1) {
		FLAG = 1;
		str = address;
		gchar *strcp = (char*)malloc(sizeof(char) * (strlen(str) + 1));
		strcpy(strcp, str);
		NUM = ati(strcp);
		free(strcp);
		(*(container->b)) = make_board((container->table), NUM, container);
		if((gtk_notebook_insert_page(GTK_NOTEBOOK((*(container->notebook))), container->table->vbox4, NULL, -1)) == -1)
			printf("failed\n");
		gtk_widget_show_all((*(container->notebook))); //to show dynamically created widgets
		switch_page(NULL, GTK_NOTEBOOK(*(container->notebook)));
	}
	else
		printf("incorrect dimension\n");
}
void add_page3(containers *container, p3widgets *widgets) {
	*(container->vbox3) = gtk_vbox_new(TRUE, 5);
	widgets->p3widgets[0] = gtk_label_new("SELECT PLAYERS");
	widgets->p3widgets[1] = gtk_button_new_with_label("TWO PLAYER");
	widgets->p3widgets[2] = gtk_button_new_with_label("THREE PLAYER");
	widgets->p3widgets[3] = gtk_button_new_with_label("FOUR PLAYER");
	widgets->p3widgets[4] = gtk_button_new_with_label("FIVE PLAYER");
	widgets->p3align[0] = gtk_alignment_new(0.5, 0.3, 0.6, 0.7);
	widgets->p3align[1] = gtk_alignment_new(0.5, 0.3, 0.35, 0.35);
	widgets->p3align[2] = gtk_alignment_new(0.5, 0.3, 0.35, 0.35);
	widgets->p3align[3] = gtk_alignment_new(0.5, 0.3, 0.35, 0.35);
	widgets->p3align[4] = gtk_alignment_new(0.5, 0.3, 0.35, 0.35);
	gtk_container_add(GTK_CONTAINER(widgets->p3align[0]), widgets->p3widgets[0]);
	gtk_container_add(GTK_CONTAINER(widgets->p3align[1]), widgets->p3widgets[1]);
	gtk_container_add(GTK_CONTAINER(widgets->p3align[2]), widgets->p3widgets[2]);
	gtk_container_add(GTK_CONTAINER(widgets->p3align[3]), widgets->p3widgets[3]);
	gtk_container_add(GTK_CONTAINER(widgets->p3align[4]), widgets->p3widgets[4]);
	gtk_box_pack_start(GTK_BOX(*(container->vbox3)), widgets->p3align[0], TRUE, TRUE, 10);		//packing for page3
	gtk_box_pack_start(GTK_BOX(*(container->vbox3)), widgets->p3align[1], TRUE, TRUE, 10);
	gtk_box_pack_start(GTK_BOX(*(container->vbox3)), widgets->p3align[2], TRUE, TRUE, 10);
	gtk_box_pack_start(GTK_BOX(*(container->vbox3)), widgets->p3align[3], TRUE, TRUE, 10);
	gtk_box_pack_start(GTK_BOX(*(container->vbox3)), widgets->p3align[4], TRUE, TRUE, 10);
	g_signal_connect(G_OBJECT(widgets->p3widgets[1]), "clicked", G_CALLBACK(select_players), &TWO);	
	g_signal_connect(G_OBJECT(widgets->p3widgets[2]), "clicked", G_CALLBACK(select_players), &THREE);
	g_signal_connect(G_OBJECT(widgets->p3widgets[3]), "clicked", G_CALLBACK(select_players), &FOUR);
	g_signal_connect(G_OBJECT(widgets->p3widgets[4]), "clicked", G_CALLBACK(select_players), &FIVE);	
	g_signal_connect(G_OBJECT(widgets->p3widgets[1]), "clicked", G_CALLBACK(switch_page), (gpointer)(*(container->notebook)));	
	g_signal_connect(G_OBJECT(widgets->p3widgets[2]), "clicked", G_CALLBACK(switch_page), (gpointer)(*(container->notebook)));
	g_signal_connect(G_OBJECT(widgets->p3widgets[3]), "clicked", G_CALLBACK(switch_page), (gpointer)(*(container->notebook)));
	g_signal_connect(G_OBJECT(widgets->p3widgets[4]), "clicked", G_CALLBACK(switch_page), (gpointer)(*(container->notebook)));
	gtk_notebook_append_page(GTK_NOTEBOOK((*(container->notebook))), *(container->vbox3), NULL);
}
void select_players(GtkWidget *button, gint *i) {
	T_PLAYERS = *i;
	ball_colors();
}
//SWITCHING OF NOTEBOOK PAGES
static void switch_page(GtkButton *button, GtkNotebook *notebook) {
	gint i = gtk_notebook_get_current_page(notebook);
	gint total = gtk_notebook_get_n_pages(notebook);
	if(i == 0)
		gtk_notebook_set_current_page(notebook, 1);
	else if(i == 1)
		gtk_notebook_set_current_page(notebook, 2);
	else if(i == 2 && total == 5)
		gtk_notebook_set_current_page(notebook, 4);
	else {
		gtk_notebook_set_current_page(notebook, 3);
	}
}
//FREE MALLOCED BUTTONS AND DESTROY WINDOW
static void destroy(GtkWidget *window, table_container *table) {
	if(FLAG)
		free_table(table);	
	gtk_main_quit();
}
void free_table(table_container *table) {
	gint i;	
	for(i = 0; i < table->childs; i++)
		g_slice_free1(sizeof(sizeof(box)), (table->b)[i]);
	g_slice_free1(sizeof(box*), table->b);
}
//MAKE GAME BOARD
box **make_board(table_container *table, gint num, containers *container) {
	int fd, data, num1 = num;
	CONTAINER = container;
	if(SAVE) {
		fd = open("save", O_RDONLY, S_IRUSR);
		read(fd, &data, sizeof(int));
		T_PLAYERS = data;
		read(fd, &data, sizeof(int));
		num1 = data;
	}
	gint i, j;
	table->vbox4 = gtk_vbox_new(FALSE, 2);
	table->hbox4 = gtk_hbox_new(FALSE, 3);
	table->widgets.pause[0] = gtk_button_new();
	table->widgets.save[0] = gtk_button_new();
	table->widgets.quit[0] = gtk_button_new();
	table->widgets.new[0] = gtk_button_new();
	//for various images
	table->widgets.pause[1] = gtk_image_new_from_stock(GTK_STOCK_MEDIA_PAUSE, GTK_ICON_SIZE_MENU);
	table->widgets.save[1] = gtk_image_new_from_stock(GTK_STOCK_SAVE, GTK_ICON_SIZE_MENU);
	table->widgets.quit[1] = gtk_image_new_from_stock(GTK_STOCK_QUIT, GTK_ICON_SIZE_MENU);
	table->widgets.new[1] = gtk_image_new_from_stock(GTK_STOCK_HOME, GTK_ICON_SIZE_MENU);
	//to add images to buttons
	gtk_container_add(GTK_CONTAINER(table->widgets.pause[0]), table->widgets.pause[1]);
	gtk_container_add(GTK_CONTAINER(table->widgets.save[0]), table->widgets.save[1]);
	gtk_container_add(GTK_CONTAINER(table->widgets.quit[0]), table->widgets.quit[1]);
	gtk_container_add(GTK_CONTAINER(table->widgets.new[0]), table->widgets.new[1]);
	//to add the buttons to hbox4
	gtk_box_pack_end(GTK_BOX(table->hbox4), (table->widgets).quit[0], FALSE, FALSE, 1);
	gtk_box_pack_end(GTK_BOX(table->hbox4), (table->widgets).save[0], FALSE, FALSE, 1);
	gtk_box_pack_end(GTK_BOX(table->hbox4), (table->widgets).pause[0], FALSE, FALSE, 1);
	gtk_box_pack_end(GTK_BOX(table->hbox4), (table->widgets).new[0], FALSE, FALSE, 1);
	table->table = gtk_table_new(num1, num1, TRUE);
	box **b;
	b = (box**)g_slice_alloc(sizeof(box*) * num1);
	for(i = 0; i < num1; i++)
		b[i] = (box*)g_slice_alloc(sizeof(box) * num1);
	for(i = 0; i < num1; i++)
		for(j = 0; j < num1; j++) {
			b[i][j].x = i;
			b[i][j].y = j;
			b[i][j].b = b;
			if((i == 0 && j== 0) || (i == 0 && j == num1 - 1) || (i == num1 - 1 && j== 0) || (i == num1 - 1 && j == num1 - 1))
				b[i][j].cmass = 2;
			else if((i == 0 || i == num1 - 1) || (j == 0 || j == num1 - 1))
				b[i][j].cmass = 3;
			else
				b[i][j].cmass = 4;
			(b[i][j]).canvas = gtk_drawing_area_new();
			(b[i][j]).eventbox = gtk_event_box_new();
			gtk_event_box_set_above_child(GTK_EVENT_BOX((b[i][j]).eventbox), FALSE);
			gtk_widget_set_events((b[i][j]).eventbox, GDK_BUTTON_RELEASE_MASK || GDK_BUTTON_PRESS_MASK);
			gtk_container_add(GTK_CONTAINER((b[i][j]).eventbox), (b[i][j]).canvas);
			gtk_table_attach(GTK_TABLE(table->table), (b[i][j]).eventbox, j, j + 1, i, i + 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND |GTK_FILL, 0, 0);
			if(SAVE) {
				read(fd, &data, sizeof(int));
				b[i][j].player = data;
				read(fd, &data, sizeof(int));
				b[i][j].ballcount = data;
			}
			else {
				b[i][j].player = 0;
				b[i][j].ballcount = 0;
			}
			//to draw the border
			g_signal_connect(G_OBJECT((b[i][j]).canvas), "expose-event", G_CALLBACK(draw_line), (b[i][j]).canvas);
			//for adding the ball to the boxes and setting the player
			g_signal_connect(G_OBJECT((b[i][j]).eventbox), "event", G_CALLBACK(check_n_add), &(b[i][j]));
		}
	table->childs = num1;
	(table->b) = b; //containes the address of malloced array of array
	*(container->b) = b;
	gtk_box_pack_start(GTK_BOX(table->vbox4), table->hbox4, FALSE, FALSE, 1);
	gtk_box_pack_start(GTK_BOX(table->vbox4), table->table, TRUE, TRUE, 1);
	g_signal_connect(G_OBJECT(table->widgets.pause[0]), "clicked", G_CALLBACK(pause_game), container);
	g_signal_connect(G_OBJECT(table->widgets.save[0]), "clicked", G_CALLBACK(save_game), *(container->b));
	g_signal_connect(G_OBJECT(table->widgets.quit[0]), "clicked", G_CALLBACK(quit_game), container);
	g_signal_connect(G_OBJECT(table->widgets.new[0]), "clicked", G_CALLBACK(new_game), container);
	if(SAVE) {
		close(fd);
	}
	return b;
}
void ball_colors() {
	COLORS[FIRST].r = 1;
	COLORS[FIRST].g = 0;
	COLORS[FIRST].b = 0;
	COLORS[FIRST].a = 0.5;
	
	COLORS[TWO].r = 0;
	COLORS[TWO].g = 1;
	COLORS[TWO].b = 0;
	COLORS[TWO].a = 0.5;
	if(T_PLAYERS >= 3) {
		COLORS[THREE].r = 0;
		COLORS[THREE].g = 0;
		COLORS[THREE].b = 1;
		COLORS[THREE].a = 0.5;
	}
	if(T_PLAYERS >= 4) {
		COLORS[FOUR].r = 1;
		COLORS[FOUR].g = 1;
		COLORS[FOUR].b = 0;
		COLORS[FOUR].a = 0.5;
	}
	if(T_PLAYERS == 5) {
		COLORS[FIVE].r = 1;
		COLORS[FIVE].g = 0;
		COLORS[FIVE].b = 1;
		COLORS[FIVE].a = 0.5;
	}
	
}
//pause and resume game
void pause_game(GtkWidget *button, containers *container) {
	GtkWidget *dialog, *label, *image, *hbox;
	dialog = gtk_dialog_new_with_buttons("PAUSE", GTK_WINDOW(*(container->window)), GTK_DIALOG_MODAL, "RESUME", GTK_RESPONSE_YES, "QUIT", GTK_RESPONSE_NO, NULL);
	gtk_dialog_set_has_separator (GTK_DIALOG (dialog), FALSE);
	label = gtk_label_new ("WANT TO RESUME GAME");
	image = gtk_image_new_from_stock (GTK_STOCK_DIALOG_INFO,
	GTK_ICON_SIZE_DIALOG);
	hbox = gtk_hbox_new (FALSE, 5);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 10);
	gtk_box_pack_start_defaults (GTK_BOX (hbox), image);
	gtk_box_pack_start_defaults (GTK_BOX (hbox), label);
	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), hbox);
	gtk_widget_show_all (dialog);
	/* Create the dialog as modal and destroy it when a button is clicked. */
	gint RESPONSE = gtk_dialog_run(GTK_DIALOG (dialog));
	if(RESPONSE == GTK_RESPONSE_YES) {
		PAUSE = 0;
		gtk_widget_destroy (dialog);
		return;
	}
	else {
		gtk_widget_destroy (dialog);
		PAUSE = 1;
		quit_game(NULL, container);
	}
}
void resume_game(GtkWidget *button, containers *container) {
	gtk_widget_set_sensitive(container->table->table, TRUE);
}
//saving the current state of game
void save_game(GtkWidget *button, box **b) {
	int fd, i, j, num1 = NUM;
	fd = open("save", O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR);
	write(fd, &(T_PLAYERS), sizeof(int));
	write(fd, &(num1), sizeof(int));
	for(i = 0; i < num1; i++) {
		for(j = 0; j < num1; j++) {
			write(fd, &(b[i][j].player), sizeof(int));
			write(fd, &(b[i][j].ballcount), sizeof(int));
		}
	}
	close(fd);
}
void new_game(GtkWidget *button, containers *container) {
	free_table(container->table);
	FIRST = 1;
	TWO = 2;
	THREE = 3;
	FOUR = 4;
	FIVE = 5;
	gint n = gtk_notebook_get_n_pages(GTK_NOTEBOOK(*(container->notebook)));
	if(n == 4)
		gtk_notebook_remove_page(GTK_NOTEBOOK(*(container->notebook)), 3);
	else
		gtk_notebook_remove_page(GTK_NOTEBOOK(*(container->notebook)), 4);
	gtk_notebook_set_current_page(GTK_NOTEBOOK(*(container->notebook)), 0);
}
void draw_line(GtkWidget *widget, GdkEvent *event, GtkWidget *canvas) {
	cairo_t *cr;
	double width = canvas->allocation.width;
	double height = canvas->allocation.height;
	cr = gdk_cairo_create(canvas->window);
	cairo_set_source_rgba(cr, 0.07, 0.07, 0.07, 1);
	cairo_rectangle(cr, 3, 3, width - 3, height - 3);	
	cairo_fill(cr);
	cairo_destroy(cr);
	cr = gdk_cairo_create(canvas->window);
	cairo_set_line_width(cr, 3);
	cairo_set_source_rgb(cr, 0, 1, 0);
	cairo_rectangle(cr, 0, 0, width, height);
	cairo_stroke(cr);
	cairo_destroy(cr);
}
void check_n_add(GtkWidget *widget, GdkEvent *event, box *b) {	
	static gint i = GDK_BUTTON_RELEASE;
	if(event->type == GDK_BUTTON_PRESS)
		i = GDK_BUTTON_PRESS;
	if(event->type == GDK_BUTTON_RELEASE) {
		if( i == GDK_BUTTON_PRESS) {
			i = GDK_BUTTON_RELEASE;
			set_player_n_add_ball(b);
		}
	}	
}
void set_player_n_add_ball(box *b) {
	static gint ballcount = 1;
	if((ballcount - T_PLAYERS) == 1)
		ballcount = 1;
	if(b->player == 0) {
		b->player = ballcount;
		(b->ballcount)++;
		(BOX_COUNT[b->player])++;
		draw_ball(b, b->player);
		ballcount++;
	}	
	else if(b->player == ballcount) {
		(b->ballcount)++;
		if(b->ballcount == b->cmass) {
			check_all(b->b);
			CURR_COUNT = ballcount;
			explode(b->b, b->x, b->y, b->player);
			CH = 0;
			remove_player(b->b, REMOVE);
			if(T_PLAYERS == 1)
				quit_game(NULL, CONTAINER);	
		}
		else {
			draw_ball(b, b->player);
		}
		ballcount++;
	}
}
void clear_box(GtkWidget *canvas, cairo_t *cr) {
	double width = canvas->allocation.width;
	double height = canvas->allocation.height;
	cr = gdk_cairo_create(canvas->window);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_rectangle(cr, 3, 3, width - 3, height - 3);
	cairo_fill(cr);
	cairo_destroy(cr);
	cr = gdk_cairo_create(canvas->window);
	cairo_set_source_rgba(cr, 0.07, 0.07, 0.07, 1);
	cairo_rectangle(cr, 3, 3, width - 3, height - 3);
	cairo_fill(cr);
	cairo_destroy(cr);
}
void draw_ball(box *b, gint player) {
	if(b->ballcount == 1) {
		draw_1_ball(b->canvas, player);
	}
	if(b->ballcount == 2) {
		draw_2_ball(b->canvas, player);
	}
	if(b->ballcount == 3) {
		draw_3_ball(b->canvas, player);
	}
}
void draw_1_ball(GtkWidget *canvas, gint player) {
	cairo_t *cr = NULL;
	clear_box(canvas, cr);
	double width = canvas->allocation.width;
	double height = canvas->allocation.height;
	cr = gdk_cairo_create(canvas->window);
	cairo_set_source_rgba(cr, COLORS[player].r, COLORS[player].g, COLORS[player].b, COLORS[player].a);
	cairo_translate(cr, width / 2, height / 2);
	cairo_arc(cr, 0, 0, width / 11, 0, 2 * M_PI);
	cairo_fill(cr);	
	cairo_destroy(cr);
}
void draw_2_ball(GtkWidget *canvas, gint player) {
	cairo_t *cr = NULL;
	clear_box(canvas, cr);
	double width = canvas->allocation.width;
	double height = canvas->allocation.height;
	cr = gdk_cairo_create(canvas->window);
	cairo_set_source_rgba(cr, COLORS[player].r, COLORS[player].g, COLORS[player].b, COLORS[player].a);
	cairo_translate(cr, width / 2, height / 2);
	cairo_arc(cr, -width / 14, 0, width / 12, 0, 2 * M_PI);
	cairo_arc(cr, width / 14, 0, width / 12, 0, 2 * M_PI);
	cairo_fill(cr);
	cairo_destroy(cr);
}
void draw_3_ball(GtkWidget *canvas, gint player) {
	cairo_t *cr = NULL;
	clear_box(canvas, cr);
	double width = canvas->allocation.width;
	double height = canvas->allocation.height;
	cr = gdk_cairo_create(canvas->window);
	cairo_set_source_rgba(cr, COLORS[player].r, COLORS[player].g, COLORS[player].b, COLORS[player].a);
	cairo_translate(cr, width / 2, height / 2);
	cairo_arc(cr, -width / 15, 0, width / 13, 0, 2 * M_PI);
	cairo_arc(cr, width / 15, 0, width / 13, 0, 2 * M_PI);
	cairo_arc(cr, 0, -height / 10, width / 13, 0, 2 * M_PI);
	cairo_fill(cr);	
	cairo_destroy(cr);
}
//IN THIS "b" REFERS TO THE CLICKED BOX
//explode(arguments are: pointer pointing to the the all boxes and x and y coordinates and b->player);
//IN THIS FUNTION "b" IS POINTER TO ALL BOXES
void add_ball(box *b, gint x, gint y, gint player) {	
	(b->ballcount)++;
	(b->b)[x][y].player = player;		
	if(b->ballcount == b->cmass) {
		explode(b->b, x, y, player);
	}
	else
		draw_ball(b, player);
}
int check_all(box **b) {
	int i, j = 0;
	for(i = 1; i <= T_PLAYERS; i++) {
		if(BOX_COUNT[i] == 0)	{
			REMOVE[j] = i;
			j++;
		}
	}
	return 0;
}
void remove_player(box **b, int *rm) {
	int i, j;
	for( ; *rm != 0; rm++) {
		for(i = 0; i < NUM; i++) {
			for(j = 0; j < NUM; j++) {
				if(b[i][j].player > *rm) {
					(b[i][j].player)--;
				}
			}
		}
		for(i = *rm; i < T_PLAYERS; i++) {
			BOX_COUNT[i] = BOX_COUNT[i + 1];
		}
		for(i = *rm; i < T_PLAYERS; i++) {
			COLORS[i] = COLORS[i + 1];
		}
		(T_PLAYERS)--;
	}
	if(T_PLAYERS == 1)
		printf("player wins");
}
void explode1(box **b, gint x, gint y, gint player);
void explode2(box **b, gint x, gint y, gint player);
void explode3(box **b, gint x, gint y, gint player);
void explode4(box **b, gint x, gint y, gint player);
void explode(box **b, gint x, gint y, gint player) {
	/*if((x == 0 && y == NUM - 1) || CH) {
		CH = 1;
		explode4(b, x, y, player);
		explode3(b, x, y, player);
		explode2(b, x, y, player);
		explode1(b, x, y, player);
	}
	else {*/
		explode1(b, x, y, player);
		explode2(b, x, y, player);
		explode3(b, x, y, player);
		explode4(b, x, y, player);
	//}
}
void explode1(box **b, gint x, gint y, gint player) {
	cairo_t *cr = NULL;
	if((x - 1) >= 0) {
		(b[x][y].ballcount)--;
		if((b[x - 1][y].player) != player) {
			(BOX_COUNT[b[x - 1][y].player])--;
			(BOX_COUNT[player])++;
		}
		if((b[x][y].ballcount) == 0) {
			BOX_COUNT[player]--;
			b[x][y].player = 0;
		}
		check_all(b);
		add_ball(&(b[x - 1][y]), x - 1, y, player);
		if((b[x][y].ballcount) == 0) {
			clear_box(b[x][y].canvas, cr);
			return;
		}
	}
}
void explode2(box **b, gint x, gint y, gint player) {
	cairo_t *cr = NULL;
	if((y + 1) < NUM) {
		(b[x][y].ballcount)--;
		if((b[x][y + 1].player) != player) {
			(BOX_COUNT[b[x][y + 1].player])--;
			(BOX_COUNT[player])++;
		}
		if((b[x][y].ballcount) == 0) {
			BOX_COUNT[player]--;
			b[x][y].player = 0;
		}
		check_all(b);
		add_ball(&(b[x][y + 1]), x, y + 1, player);
		if((b[x][y].ballcount) == 0) {
			clear_box(b[x][y].canvas, cr);
			return;
		}
	}
}
void explode3(box **b, gint x, gint y, gint player) {
	cairo_t *cr = NULL;
	if((x + 1) < NUM) {
		(b[x][y].ballcount)--;
		if((b[x + 1][y].player) != player) {
			(BOX_COUNT[b[x + 1][y].player])--;
			(BOX_COUNT[player])++;
		}
		if((b[x][y].ballcount) == 0) {
			BOX_COUNT[player]--;
			b[x][y].player = 0;
		}
		check_all(b);
		add_ball(&(b[x + 1][y]), x + 1, y, player);
		if((b[x][y].ballcount) == 0) {
			clear_box(b[x][y].canvas, cr);
			return;
		}
	}	
}
void explode4(box **b, gint x, gint y, gint player) {
	cairo_t *cr = NULL;
	if((y - 1) >= 0) {
		(b[x][y].ballcount)--;
		if((b[x][y - 1].player) != player) {
			(BOX_COUNT[b[x][y - 1].player])--;
			(BOX_COUNT[player])++;
		}
		if((b[x][y].ballcount) == 0) {
			BOX_COUNT[player]--;
			b[x][y].player = 0;
		}
		check_all(b);
		add_ball(&(b[x][y - 1]), x, y - 1, player);
		if((b[x][y].ballcount) == 0) {
			clear_box(b[x][y].canvas, cr);
			return;
		}
	}
}
int main(int argc, char *argv[]) {
	GtkWidget *window, *notebook = NULL, *vbox1 = NULL, *vbox2 = NULL, *vbox3 = NULL;	//containers
	table_container table;
	p1widgets widgets1;	//page 1 widgets
	p2widgets widgets2;	//page 2 widgets
	p3widgets widgets3;	//page 3 widgets
	box **b;
	containers container;
	pack_all_containers(&container, &window, &notebook, &vbox1, &vbox2, &vbox3, &table, &b);
	gtk_rc_parse("style.rc");
	//all initialization
	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "NOTEBOOK");
	gtk_widget_set_size_request(window, 800, 600);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	notebook = gtk_notebook_new();
	add_page1(&container, &widgets1);
	add_page2(&container, &widgets2);	
	add_page3(&container, &widgets3);	
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroy), &table);
	g_signal_connect(G_OBJECT(*(container.notebook)), "expose-event", G_CALLBACK(draw_balls), &container);
	gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), FALSE);
	gtk_container_add(GTK_CONTAINER(window), notebook);	
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}
