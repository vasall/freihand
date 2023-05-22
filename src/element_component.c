#include "element_component.h"

#include "document.h"
#include "view.h"

#include <stdlib.h>

/*
 * 
 * 		CREATE
 *
 */

FH_INTERN s8 comp_create_canvas(struct fh_component *c)
{
	struct fh_rect rect = fh_GetElementShape(c->element);
	struct fh_view_list *lst;

	printf("Document: %p\n", c->element->document);
	printf("Views: %p\n", c->element->document->views);

	lst = c->element->document->views;

	if(!(c->ref = fh_CreateView(lst, &rect)))
		goto err_return;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to create canvas component");
	return -1;
}


/*
 * 
 * 		DESTROY
 *
 */

FH_INTERN void comp_destroy_canvas(struct fh_component *c)
{
	fh_DestroyView(c->ref);
}


/*
 * 
 * 		UPDATE
 *
 */

FH_INTERN void comp_update_canvas(struct fh_component *c)
{
	struct fh_rect rect = fh_GetElementShape(c->element);

	fh_ResizeView(c->ref, &rect);
}


/*
 * 
 * 		RENDER
 *
 */

FH_INTERN void comp_render_canvas(struct fh_component *c)
{
	fh_RenderView(c->ref);
}

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */

FH_API struct fh_component *fh_CreateComponent(struct fh_element *ele,
		enum fh_component_type type)
{
	struct fh_component *comp;
	s8 r = -1;

	if(!ele) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	if(!(comp = fh_malloc(sizeof(struct fh_component)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for component");
		goto err_return;
	}

	/* Set attributes */
	comp->element = ele;
	comp->type = type;

	switch(type) {
		case FH_COMPONENT_CANVAS: r = comp_create_canvas(comp); break;
		default: ALARM(ALARM_ERR, "Type not found"); break;
	}

	if(r < 0) goto err_free_comp;

	return comp;

err_free_comp:
	fh_free(comp);

err_return:
	ALARM(ALARM_ERR, "Failed to create component");
	return NULL;
}


FH_API void fh_DestroyComponent(struct fh_component *comp)
{
	if(!comp) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	switch(comp->type) {
		case FH_COMPONENT_CANVAS: comp_destroy_canvas(comp); break;	
	}

	fh_free(comp);
}


FH_API void fh_UpdateComponent(struct fh_component *comp, void *data)
{
	if(!comp) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_Ignore(data);

	switch(comp->type) {
		case FH_COMPONENT_CANVAS: comp_update_canvas(comp); break;
	}
}


FH_API void fh_RenderComponent(struct fh_component *comp)
{
	if(!comp) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	switch(comp->type) {
		case FH_COMPONENT_CANVAS: comp_render_canvas(comp); break;
	}
}
