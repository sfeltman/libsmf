
EXTRA_DIST = \
	test_python.py \
	chpn_op53.mid \
	COPYING

RUN_TESTS_ENV_VARS= \
	LD_LIBRARY_PATH=$(top_builddir)/src/.libs:$$LD_LIBRARY_PATH \
	GI_TYPELIB_PATH=$(top_builddir)/src:$$GI_TYPELIB_PATH \
	MALLOC_PERTURB_=85 \
	MALLOC_CHECK_=3 \
	G_SLICE=debug-blocks

check-local:
	@echo "======================================================================"
	@(`which python` --version)
	$(RUN_TESTS_ENV_VARS) `which python` -m unittest discover --verbose --start-directory $(srcdir)
