/* ------------------------ fatom ----------------------------- */

#define x_val a_pos.a_w.w_float
#define DEBUG(x)

typedef struct _fatom
{
     t_object x_obj;
     t_atom a_pos;

     t_glist * x_glist;
     int x_rect_width;
     int x_rect_height;
     t_symbol*  x_sym;
     t_symbol*  x_type;

     int x_max;
     int x_min;
     int x_width;
} t_fatom;

/* widget helper functions */




static void draw_inlets(t_fatom *x, t_glist *glist, int firsttime, int nin, int nout)
{
     int n = nin;
     int nplus, i;
     nplus = (n == 1 ? 1 : n-1);
     DEBUG(post("draw inlet");)
     for (i = 0; i < n; i++)
     {
	  int onset = text_xpix(&x->x_obj, glist) + (x->x_rect_width - IOWIDTH) * i / nplus;
	  if (firsttime)
	       sys_vgui(".x%x.c create rectangle %d %d %d %d -tags %xo%d\n",
			glist_getcanvas(glist),
			onset, text_ypix(&x->x_obj, glist) + x->x_rect_height - 1,
			onset + IOWIDTH, text_ypix(&x->x_obj, glist) + x->x_rect_height,
			x, i);
	  else
	       sys_vgui(".x%x.c coords %xo%d %d %d %d %d\n",
			glist_getcanvas(glist), x, i,
			onset, text_ypix(&x->x_obj, glist) + x->x_rect_height - 1,
			onset + IOWIDTH, text_ypix(&x->x_obj, glist) + x->x_rect_height);
     }
     n = nout; 
     nplus = (n == 1 ? 1 : n-1);
     for (i = 0; i < n; i++)
     {
	  int onset = text_xpix(&x->x_obj, glist) + (x->x_rect_width - IOWIDTH) * i / nplus;
	  if (firsttime)
	       sys_vgui(".x%x.c create rectangle %d %d %d %d -tags %xi%d\n",
			glist_getcanvas(glist),
			onset, text_ypix(&x->x_obj, glist),
			     onset + IOWIDTH, text_ypix(&x->x_obj, glist) + 1,
			x, i);
	  else
	       sys_vgui(".x%x.c coords %xi%d %d %d %d %d\n",
			glist_getcanvas(glist), x, i,
			onset, text_ypix(&x->x_obj, glist),
			onset + IOWIDTH, text_ypix(&x->x_obj, glist) + 1);
	  
     }
     DEBUG(post("draw inlet end");)
}


static void draw_handle(t_fatom *x, t_glist *glist, int firsttime) {
  int onset = text_xpix(&x->x_obj, glist) + (x->x_rect_width - IOWIDTH+2);

  if (firsttime)
    sys_vgui(".x%x.c create rectangle %d %d %d %d -tags %xhandle\n",
	     glist_getcanvas(glist),
	     onset, text_ypix(&x->x_obj, glist) + x->x_rect_height - 12,
	     onset + IOWIDTH, text_ypix(&x->x_obj, glist) + x->x_rect_height-4,
	     x);
  else
    sys_vgui(".x%x.c coords %xhandle %d %d %d %d\n",
	     glist_getcanvas(glist), x, 
	     onset, text_ypix(&x->x_obj, glist) + x->x_rect_height - 12,
	     onset + IOWIDTH, text_ypix(&x->x_obj, glist) + x->x_rect_height-4);
}

static void create_widget(t_fatom *x, t_glist *glist)
{
  t_canvas *canvas=glist_getcanvas(glist);
  if (!strcmp(x->x_type->s_name,"vslider")) {
    x->x_rect_width = x->x_width+15;
    x->x_rect_height =  x->x_max-x->x_min+24;
    
    sys_vgui("scale .x%x.c.s%x \
                    -sliderlength 10 \
                    -showvalue 0 \
                    -length %d \
                    -resolution 0.01 \
                    -repeatinterval 20 \
                    -from %d -to %d \
                    -width %d \
                    -command fatom_cb%x\n",canvas,x,
	     x->x_max-x->x_min+14,
	     x->x_max,
	     x->x_min,
	     x->x_width,
	     x);
  } else  if (!strcmp(x->x_type->s_name,"hslider")) {
    x->x_rect_width =  x->x_max-x->x_min + 24;
    x->x_rect_height = x->x_width + 15;
    sys_vgui("scale .x%x.c.s%x \
                    -sliderlength 10 \
                    -showvalue 0 \
                    -length %d \
                    -resolution 0.01 \
                    -orient horizontal \
                    -repeatinterval 20 \
                    -from %d -to %d \
                    -width %d \
                    -command fatom_cb%x\n",canvas,x,
	     x->x_max-x->x_min+14,
	     x->x_min,
	     x->x_max,
	     x->x_width,
	     x);
  } else if (!strcmp(x->x_type->s_name,"checkbutton")) {
       x->x_rect_width = 30;
       x->x_rect_height = 25;
       sys_vgui("checkbutton .x%x.c.s%x \
                    -command { fatom_cb%x $fatom_val%x} -variable fatom_val%x\n",canvas,x,x,x,x);
  } else if (!strcmp(x->x_type->s_name,"hradio")) {
    int i;
    x->x_rect_width = 8*20;
       x->x_rect_height = 25;
       for (i=0;i<8;i++) {
	 sys_vgui("radiobutton .x%x.c.s%x%d \
                    -command { fatom_cb%x $fatom_val%x} -variable fatom_val%x -value %d\n",canvas,x,i,x,x,x,i);
       }
       /* TODO pack them */
     } else if (!strcmp(x->x_type->s_name,"vradio")) {
       int i;
       x->x_rect_width = 30;
       x->x_rect_height = 20*8+5;
       for (i=0;i<8;i++) {
	 sys_vgui("radiobutton .x%x.c.s%x%d \
                    -command { fatom_cb%x $fatom_val%x} -variable fatom_val%x -value %d\n",canvas,x,i,x,x,x,i);
       }
       /* TODO pack them */
     } else {
       x->x_rect_width = 32;
       x->x_rect_height = 140;
       sys_vgui("scale .x%x.c.s%x \
                    -sliderlength 10 \
                    -showvalue 0 \
                    -length 131 \
                    -from 127 -to 0 \
                    -command fatom_cb%x\n",canvas,x,x);
     }     
}





static void fatom_drawme(t_fatom *x, t_glist *glist, int firsttime)
{
  t_canvas *canvas=glist_getcanvas(glist);
  DEBUG(post("drawme %d",firsttime);)
     if (firsttime) {
       DEBUG(post("glist %x canvas %x",x->x_glist,canvas);)
       if (x->x_glist != canvas) {
	 create_widget(x,glist);	       
	 x->x_glist = canvas;
       }
       sys_vgui(".x%x.c create window %d %d -anchor nw -window .x%x.c.s%x -tags %xS\n", 
		canvas,text_xpix(&x->x_obj, glist), text_ypix(&x->x_obj, glist)+2,x->x_glist,x,x);
              
     }     
     else {
       sys_vgui(".x%x.c coords %xS \
%d %d\n",
		canvas, x,
		text_xpix(&x->x_obj, glist), text_ypix(&x->x_obj, glist)+2);
     }
     draw_inlets(x, glist, firsttime, 1,1);
     draw_handle(x, glist, firsttime);

}


static void fatom_erase(t_fatom* x,t_glist* glist)
{
     int n;

     DEBUG(post("erase");)
       sys_vgui("destroy .x%x.c.s%x\n",glist_getcanvas(glist),x);

     sys_vgui(".x%x.c delete %xS\n",glist_getcanvas(glist), x);

     /* inlets and outlets */
     
     sys_vgui(".x%x.c delete %xi%d\n",glist_getcanvas(glist),x,0);
     sys_vgui(".x%x.c delete %xo%d\n",glist_getcanvas(glist),x,0);
     sys_vgui(".x%x.c delete  %xhandle\n",glist_getcanvas(glist),x,0);
}
	


/* ------------------------ fatom widgetbehaviour----------------------------- */


static void fatom_getrect(t_gobj *z, t_glist *owner,
    int *xp1, int *yp1, int *xp2, int *yp2)
{
    int width, height;
    t_fatom* s = (t_fatom*)z;

    width = s->x_rect_width;
    height = s->x_rect_height;
    *xp1 = text_xpix(&s->x_obj, owner);
    *yp1 = text_ypix(&s->x_obj, owner);
    *xp2 = text_xpix(&s->x_obj, owner) + width;
    *yp2 = text_ypix(&s->x_obj, owner) + height;
}

static void fatom_displace(t_gobj *z, t_glist *glist,
    int dx, int dy)
{
    t_fatom *x = (t_fatom *)z;
    DEBUG(post("displace");)
    x->x_obj.te_xpix += dx;
    x->x_obj.te_ypix += dy;
    if (glist_isvisible(glist))
    {
      sys_vgui(".x%x.c coords %xSEL %d %d %d %d\n",
	       glist_getcanvas(glist), x,
	       text_xpix(&x->x_obj, glist), text_ypix(&x->x_obj, glist),
	       text_xpix(&x->x_obj, glist) + x->x_rect_width, text_ypix(&x->x_obj, glist) + x->x_rect_height);
      
      fatom_drawme(x, glist, 0);
      canvas_fixlinesfor(glist_getcanvas(glist),(t_text*) x);
    }
    DEBUG(post("displace end");)
}

static void fatom_select(t_gobj *z, t_glist *glist, int state)
{
     t_fatom *x = (t_fatom *)z;
     if (state) {
	  sys_vgui(".x%x.c create rectangle \
%d %d %d %d -tags %xSEL -outline blue\n",
		   glist_getcanvas(glist),
		   text_xpix(&x->x_obj, glist), text_ypix(&x->x_obj, glist),
		   text_xpix(&x->x_obj, glist) + x->x_rect_width, text_ypix(&x->x_obj, glist) + x->x_rect_height,
		   x);
     }
     else {
	  sys_vgui(".x%x.c delete %xSEL\n",
		   glist_getcanvas(glist), x);
     }



}


static void fatom_activate(t_gobj *z, t_glist *glist, int state)
{
/*    t_text *x = (t_text *)z;
    t_rtext *y = glist_findrtext(glist, x);
    if (z->g_pd != gatom_class) rtext_activate(y, state);*/
}

static void fatom_delete(t_gobj *z, t_glist *glist)
{
    t_text *x = (t_text *)z;
    canvas_deletelinesfor(glist_getcanvas(glist), x);
}

       
static void fatom_vis(t_gobj *z, t_glist *glist, int vis)
{
    t_fatom* s = (t_fatom*)z;
    t_rtext *y;
    DEBUG(post("vis: %d",vis);)
    if (vis) {
      	y = (t_rtext *) rtext_new_without_senditup(glist, (t_text *)z, glist->gl_editor->e_rtext);
	 fatom_drawme(s, glist, 1);
    }
    else {
	y = glist_findrtext(glist, (t_text *)z);
	 fatom_erase(s,glist);
	rtext_free(y);
    }
}

static void fatom_save(t_gobj *z, t_binbuf *b);

t_widgetbehavior   fatom_widgetbehavior = {
  w_getrectfn:  fatom_getrect,
  w_displacefn: fatom_displace,
  w_selectfn:   fatom_select,
  w_activatefn: fatom_activate,
  w_deletefn:   fatom_delete,
  w_visfn:      fatom_vis,
  w_savefn:     fatom_save,
  w_clickfn:    NULL,
  w_propertiesfn: NULL,
}; 


void fatom_size(t_fatom* x,t_floatarg w,t_floatarg h) {
     x->x_rect_width = w;
     x->x_rect_height = h;
}

void fatom_color(t_fatom* x,t_symbol* col)
{
/*     outlet_bang(x->x_obj.ob_outlet); only bang if there was a bang .. 
       so color black does the same as bang, but doesn't forward the bang 
*/
}

static void fatom_float(t_fatom* x,t_floatarg f) 
{
 
    x->x_val = f;
     if (!strcmp(x->x_type->s_name,"checkbutton")) {
       if (x->x_val)
	 sys_vgui(".x%x.c.s%x select\n",x->x_glist,x,f);
       else
	 sys_vgui(".x%x.c.s%x deselect\n",x->x_glist,x,f);
     } else
       sys_vgui(".x%x.c.s%x set %f\n",x->x_glist,x,f);
     outlet_float(x->x_obj.ob_outlet,f);
}

static void fatom_f(t_fatom* x,t_floatarg f) 
{
     x->x_val = f;
     outlet_float(x->x_obj.ob_outlet,f);
}



static void fatom_save(t_gobj *z, t_binbuf *b)
{

    t_fatom *x = (t_fatom *)z;

    binbuf_addv(b, "ssiiss", gensym("#X"),gensym("obj"),
		x->x_obj.te_xpix, x->x_obj.te_ypix ,  
		gensym("fatom"),x->x_type);
    binbuf_addv(b, ";");
}


static void *fatom_new(t_fatom* x,t_floatarg max, t_floatarg min, t_floatarg h)
{
    char buf[256];
    x->x_glist = (t_glist*)NULL;


    x->a_pos.a_type = A_FLOAT;

    if (!max) x->x_max = 127; 
    else
      x->x_max = max;
    x->x_min = min;    
    if (h) x->x_width = h;
    else x->x_width = 15;
   

/*
    if (o) x->x_height = o;
    else
*/

    /* bind to a symbol for slider callback (later make this based on the
       filepath ??) */

    sprintf(buf,"fatom%x",x);
    x->x_sym = gensym(buf);
    pd_bind(&x->x_obj.ob_pd, x->x_sym);

/* pipe startup code to slitk */

    sys_vgui("proc fatom_cb%x {val} {\n
       pd [concat fatom%x f $val \\;]\n
       }\n",x,x);

    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void fatom_setup_common(t_class* class)
{

    class_addfloat(class, (t_method)fatom_float);
    class_addmethod(class, (t_method)fatom_f, gensym("f"),
    	A_FLOAT, 0);

/*
    class_addmethod(class, (t_method)fatom_size, gensym("size"),
    	A_FLOAT, A_FLOAT, 0);

    class_addmethod(class, (t_method)fatom_color, gensym("color"),
    	A_SYMBOL, 0);
*/
/*
    class_addmethod(class, (t_method)fatom_open, gensym("open"),
    	A_SYMBOL, 0);
*/
    class_setwidget(class,&fatom_widgetbehavior);

}