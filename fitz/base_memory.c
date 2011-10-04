#include "fitz.h"

void *
fz_malloc(fz_context *ctx, unsigned int size)
{
	void *p = ctx->alloc->malloc(ctx->alloc->user, size);
	if (p == NULL)
		fz_throw(ctx, "malloc of %d bytes failed", size);
	return p;
}

void *
fz_malloc_no_throw(fz_context *ctx, unsigned int size)
{
	return ctx->alloc->malloc(ctx->alloc->user, size);
}

void *
fz_malloc_array(fz_context *ctx, unsigned int count, unsigned int size)
{
	void *p;

	if (count == 0 || size == 0)
		return 0;

	if (count > UINT_MAX / size)
		fz_throw(ctx, "malloc of array (%d x %d bytes) failed (integer overflow)", count, size);

	p = ctx->alloc->malloc(ctx->alloc->user, count * size);
	if (p == NULL)
		fz_throw(ctx, "malloc of array (%d x %d bytes) failed", count, size);
	return p;
}

void *
fz_malloc_array_no_throw(fz_context *ctx, unsigned int count, unsigned int size)
{
	if (count == 0 || size == 0)
		return 0;

	if (count > UINT_MAX / size)
	{
		fprintf(stderr, "error: malloc of array (%d x %d bytes) failed (integer overflow)", count, size);
		return NULL;
	}

	return ctx->alloc->malloc(ctx->alloc->user, count * size);
}

void *
fz_calloc(fz_context *ctx, unsigned int count, unsigned int size)
{
	void *p;

	if (count == 0 || size == 0)
		return 0;

	if (count > UINT_MAX / size)
	{
		fz_throw(ctx, "calloc (%d x %d bytes) failed (integer overflow)", count, size);
	}

	p = ctx->alloc->malloc(ctx->alloc->user, count * size);
	if (p == NULL)
	{
		fz_throw(ctx, "calloc (%d x %d bytes) failed", count, size);
	}
	memset(p, 0, count*size);
	return p;
}

void *
fz_calloc_no_throw(fz_context *ctx, unsigned int count, unsigned int size)
{
	void *p;

	if (count == 0 || size == 0)
		return 0;

	if (count > UINT_MAX / size)
	{
		fprintf(stderr, "error: calloc (%d x %d bytes) failed (integer overflow)\n", count, size);
		return NULL;
	}

	p = ctx->alloc->malloc(ctx->alloc->user, count * size);
	if (p != NULL)
	{
		memset(p, 0, count*size);
	}
	return p;
}

void *
fz_resize_array(fz_context *ctx, void *p, unsigned int count, unsigned int size)
{
	void *np;

	if (count == 0 || size == 0)
	{
		fz_free(ctx, p);
		return 0;
	}

	if (count > UINT_MAX / size)
		fz_throw(ctx, "resize array (%d x %d bytes) failed (integer overflow)", count, size);

	np = ctx->alloc->realloc(ctx->alloc->user, p, count * size);
	if (np == NULL)
		fz_throw(ctx, "resize array (%d x %d bytes) failed", count, size);
	return np;
}

void *
fz_resize_array_no_throw(fz_context *ctx, void *p, unsigned int count, unsigned int size)
{
	if (count == 0 || size == 0)
	{
		fz_free(ctx, p);
		return 0;
	}

	if (count > UINT_MAX / size)
	{
		fprintf(stderr, "error: resize array (%d x %d bytes) failed (integer overflow)\n", count, size);
		return NULL;
	}

	return ctx->alloc->realloc(ctx->alloc->user, p, count * size);
}

void
fz_free(fz_context *ctx, void *p)
{
	ctx->alloc->free(ctx->alloc->user, p);
}

char *
fz_strdup(fz_context *ctx, char *s)
{
	int len = strlen(s) + 1;
	char *ns = fz_malloc(ctx, len);
	memcpy(ns, s, len);
	return ns;
}

char *
fz_strdup_no_throw(fz_context *ctx, char *s)
{
	int len = strlen(s) + 1;
	char *ns = fz_malloc_no_throw(ctx, len);
	if (ns != NULL)
		memcpy(ns, s, len);
	return ns;
}

static void *
fz_malloc_default(void *opaque, unsigned int size)
{
	return malloc(size);
}

static void *
fz_realloc_default(void *opaque, void *old, unsigned int size)
{
	return realloc(old, size);
}

static void
fz_free_default(void *opaque, void *ptr)
{
	free(ptr);
}

fz_alloc_context fz_alloc_default =
{
	NULL,
	fz_malloc_default,
	fz_realloc_default,
	fz_free_default
} ;