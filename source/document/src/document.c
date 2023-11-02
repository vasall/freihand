#include "document/inc/document.h"

#include "system/inc/system.h"

#include <stdlib.h>


FH_INTERN s8 doc_create_flat(struct fh_document *doc)
{
	struct fh_context *ctx = doc->context;
	u16 w = doc->shape_ref->w;
	u16 h = doc->shape_ref->h;
	
	printf("Create flat with w=%d, h=%d\n", w, h);

	if(!(doc->flat = fh_CreateFlat(ctx, "ui", w, h)))
		goto err_return;

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to create flat for document");
	return -1;
}


FH_INTERN void doc_destroy_flat(struct fh_document *doc)
{
	fh_DestroyFlat(doc->flat);
}


FH_INTERN s8 doc_create_ui(struct fh_document *doc)
{
	struct fh_model_c *c;

	unsigned int vtxnum = 4;
	unsigned int idxnum = 6;
	struct fh_context *ctx = doc->context;

	float vertices[] = {
		-1,  -1,   0,
		-1,   1,   0,
		 1,   1,   0,
		 1,  -1,   0
	};

	/* Texture coordinates (2 floats per vertex) */
	float texCoords[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	/* Indices (3 ints per triangle) */
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	vec3_t ini_pos = {0, 0, 0};
	vec3_t ini_rot = {0, 0, 0};

	if(!(c = fh_BeginModelConstr("ui", vtxnum, idxnum, indices))) {
		printf("Failed to begin construction\n");
		goto err_return;
	}

	fh_ModelConstrShader(c, fh_GetShader(ctx, "flat"));
	fh_ModelConstrTexture(c, fh_GetTexture(ctx, "ui"));

	fh_ModelConstrAttrib(c, "v_pos", 3, GL_FLOAT, vertices);
	fh_ModelConstrAttrib(c, "v_uv", 2, GL_FLOAT, texCoords);

	if(!(doc->ui = fh_EndModelConstr(c, ctx, ini_pos, ini_rot))) {
		printf("Failed to finalize construction\n");
		goto err_return;
	}

	fh_ModelConstrCleanup(c);	

	return 0;

err_return:
	ALARM(ALARM_ERR, "Failed to create UI model for document");
	return -1;
}


/*
 * 
 *     CALLBACK-FUNCTIONS
 *
 */

FH_INTERN s8 doc_cfnc_remove(struct fh_element *ele, void *data)
{
	fh_Ignore(data);

	/* First detach the element */
	fh_DetachElement(ele);

	/* Then destroy it */
	fh_DestroyElement(ele);

	return 0;
}

FH_INTERN s8 doc_cfnc_find(struct fh_element *ele, void *data)
{
	struct fh_ele_selector *sel = (struct fh_ele_selector *)data;

	if(sel->state == 1)
		return 1;

	if(strcmp(ele->name, sel->name) == 0) {
		sel->state = 1;
		sel->element = ele;
		return 1;
	}

	return 0;
}

FH_INTERN s8 doc_cfnc_findpos(struct fh_element *ele, void *data)
{
	struct fh_ele_selector *sel = (struct fh_ele_selector *)data;
	struct fh_rect rect;

	rect = fh_GetElementBox(ele);

	if(sel->state == 1)
		return 1;

	if(sel->pos->x < rect.x || sel->pos->x > (rect.x + rect.w))
		return 0;

	if(sel->pos->y < rect.y || sel->pos->y > (rect.y + rect.h))
		return 0;

	sel->element = ele;
	sel->state = 1;

	return 1;
}


FH_INTERN s8 doc_cfnc_update_style(struct fh_element *ele, void *data)
{
	fh_Ignore(data);

	fh_UpdateElementStyle(ele);

	return 0;
}


FH_INTERN s8 doc_cfnc_update_shape(struct fh_element *ele, void *data)
{
	fh_Ignore(data);

	fh_UpdateElementChildrenShape(ele);

	return 0;
}


FH_INTERN s8 doc_cfnc_render_ui(struct fh_element *ele, void *data)
{
	fh_Ignore(data);

	fh_element_render(ele);

	return 0;
}


FH_INTERN s8 doc_cfnc_render_ui_post(struct fh_element *ele, void *data)
{
	fh_Ignore(data);

	fh_element_ren_scrollbar(ele);

	return 0;
}


FH_INTERN s8 doc_cfnc_show(struct fh_element  *ele, void *data)
{
	u8 i;

	s32 lim = 16;

	struct fh_rect r;

	fh_Ignore(data);

	for(i = 0; i < ele->layer; i++) {
		printf("  ");
		lim -= 2;
	}

	printf("%s ", ele->name);
	lim -= strlen(ele->name);

	for(i = 0; i < lim; i++)
		printf(" ");


	r = fh_GetBoundingBox(ele);
	fh_rect_dump(&r);
	printf("  --  ");
	r = fh_GetElementBox(ele);
	fh_rect_dump(&r);
	printf("  --  ");
	r = fh_GetContentBox(ele);
	fh_rect_dump(&r);

	printf("\n");

	return 0;
	
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *				APPLICATION-INTERFACE
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 */


FH_API struct fh_document *fh_CreateDocument(struct fh_window *win)
{
	struct fh_document *doc;

	/* Allocate memory for the document */
	if(!(doc = fh_zalloc(sizeof(struct fh_document)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for document");
		goto err_return;
	}

	/* Set the attributes for the document */
	doc->window = win;
	doc->context = win->context;

	/* Get a reference to the shape/size of the window */
	doc->shape_ref = &win->shape;

	/* Create the body element */	
	if(!(doc->body = fh_CreateElement(doc, "body", FH_BODY, NULL))) {
		ALARM(ALARM_ERR, "Failed to create body for document");
		goto err_free_doc;
	}

	/* Set the attributes for the body element */
	doc->body->body = doc->body;
	doc->body->parent = NULL;
	doc->body->layer = 0;

	doc->selected	= NULL;
	doc->hovered	= NULL;

	/* Create the flat */
	if(doc_create_flat(doc) < 0)
		goto err_destroy_body;

	/* Create the UI model */
	if(doc_create_ui(doc) < 0)
		goto err_destroy_flat;

	/* Create the view list */
	if(!(doc->views = fh_CreateViewList(doc->context)))
		goto err_destroy_ui;

	/* Update the body element */
	fh_UpdateDocument(doc);
	
	/* Render the UI for the first time */
	fh_RenderDocumentUI(doc);

	return doc;

err_destroy_ui:
	fh_RemoveModel(doc->ui);	

err_destroy_flat:
	fh_DestroyFlat(doc->flat);

err_destroy_body:
	fh_DestroyElement(doc->body);

err_free_doc:
	fh_free(doc);

err_return:
	ALARM(ALARM_ERR, "Failed to create new document");
	return NULL;
}


FH_API void fh_DestroyDocument(struct fh_document *doc)
{
	if(!doc)
		return;

	/* Destroy the UI flat */
	doc_destroy_flat(doc);
	

	/* If the document contains a body, recursivly remove it */
	fh_RemoveElement(doc, doc->body);	

	/* Then destroy all left over views */
	fh_DestroyViewList(doc->views);

	fh_free(doc);
}


FH_API void fh_ResizeDocument(struct fh_document *doc)
{
	if(!doc) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	/* First resize the flat */
	fh_ResizeFlat(doc->flat, doc->shape_ref->w, doc->shape_ref->h);

	/* Then resize the elements */
	fh_UpdateDocument(doc);
	fh_RenderDocumentUI(doc);
}


FH_API struct fh_element *fh_AddElement(struct fh_document *doc,
		struct fh_element *parent, char *name,
		enum fh_element_type type, void *data)
{
	struct fh_element *ele;

	if(!doc || !parent || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* Create new element */
	if(!(ele = fh_CreateElement(doc, name, type, data))) {
		ALARM(ALARM_ERR, "Failed to create new element for document");
		goto err_return;
	}

	/* Attach element to parent */
	if(fh_AttachElement(parent, ele) < 0) {
		ALARM(ALARM_ERR, "Failed to attach element to parent");
		goto err_destroy_ele;
	}

	/* Update the new element */
	fh_UpdateDocumentBranch(doc, ele);
	fh_RenderDocumentUIBranch(doc, ele);

	return ele;

err_destroy_ele:
	fh_DestroyElement(ele);

err_return:
	ALARM(ALARM_ERR, "Failed to add element to document");
	return NULL;
}


FH_API void fh_RemoveElement(struct fh_document *doc, struct fh_element *ele)
{
	if(!doc || !ele) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_element_hlf(ele, &doc_cfnc_remove, NULL, NULL);
}


FH_API struct fh_element *fh_GetElement(struct fh_document *doc, char *name)
{
	struct fh_ele_selector sel;

	if(!doc || !name) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	sel.state = 0;
	sel.name = name;
	sel.element = NULL;
	
	/* Recursivly search for the element in the document */
	fh_element_hlf(doc->body, NULL, &doc_cfnc_find, &sel);

	if(sel.state == 1) {
		return sel.element;
	}

err_return:
	ALARM(ALARM_ERR, "Failed to get element");
	return NULL;
}


FH_API struct fh_element *fh_GetHoveredElement(struct fh_document *doc,
		struct fh_sin2 *pos)
{
	struct fh_ele_selector sel;

	if(!doc) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	sel.state = 0;
	sel.pos = pos;
	sel.element = NULL;

	fh_element_hlf(doc->body, NULL, &doc_cfnc_findpos, &sel);

	if(sel.state == 1) {
		return sel.element;
	}

	return NULL;

err_return:
	ALARM(ALARM_ERR, "Failed to get hovered element");
	return NULL;
}


FH_API void fh_UpdateDocumentBranch(struct fh_document *doc,
		struct fh_element *ele)
{
	if(!doc || !ele) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	/* First update the style */
	fh_element_hlf(ele, &doc_cfnc_update_style, NULL, NULL);

	/* 
	 * Then update the shape of the element. Note that this works in a very
	 * specifiy way. The function to update the shape will apply the new
	 * layout to the children-elements. Because of that, we have to call the
	 * function with the parent element. To update the position and shape of
	 * the wanted element.
	 */
	if(!ele->parent) {
		fh_element_calc_render_rect(ele);

		fh_element_hlf(ele, &doc_cfnc_update_shape, NULL, NULL);
	}
	else {
		fh_element_hlf(ele->parent, &doc_cfnc_update_shape, NULL, NULL);
	}
}


FH_API void fh_UpdateDocument(struct fh_document *doc)
{
	if(!doc) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_UpdateDocumentBranch(doc, doc->body);
}


FH_API void fh_RenderDocumentUIBranch(struct fh_document *doc,
		struct fh_element *ele)
{
	struct fh_rect r;

	if(!doc || !ele) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_element_hlf(ele, &doc_cfnc_render_ui, &doc_cfnc_render_ui_post,
			NULL);

	r = fh_GetBoundingBox(ele);
	fh_UpdateFlat(doc->flat, &r);
	
}


FH_API void fh_RenderDocumentUI(struct fh_document *doc)
{
	if(!doc) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_RenderDocumentUIBranch(doc, doc->body);

}


FH_API void fh_RenderDocument(struct fh_document *doc)
{
	if(!doc) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	fh_RenderViewList(doc->views);
	fh_RenderModel(doc->ui, NULL, NULL);

}


FH_API void fh_ShowDocumentTree(struct fh_document *doc,
		struct fh_element *ele)
{
	struct fh_element *start;

	if(!doc) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}

	if(ele)
		start = ele;
	else
		start = doc->body;

	fh_element_hlf(start, &doc_cfnc_show, NULL, NULL);
}