
bin_PROGRAMS = vertstatsinfo

include_HEADERS = mniVolume.h \
	mniLabelVolume.h \
	mniBaseVolume.h \
	mniVertstatsFile.h

LDADD = liboobicpl.la

lib_LTLIBRARIES = liboobicpl.la

TESTS = vertstats_tests \
	single_column_file_test \
	multiple_columns_file_test

check_PROGRAMS = $(TESTS)

liboobicpl_la_SOURCES = mniBaseVolume.cc \
	mniVolume.cc \
	mniLabelVolume.cc \
	mniVertstatsFile.cc 

vertstatsinfo_SOURCES = vertstatsinfo.cc

negative_values_test_SOURCES = negativeValuesTest.cc
#negative_values_test_LDADD = -L$(libdir) -loobicpl
volume_tests_SOURCES = volumeTests.cc
#volume_tests_LDADD = oobicpl.la

vertstats_tests_SOURCES = vertstatsTests.cc
#vertstats_tests_LDADD = liboobicpl.la

single_column_file_test_SOURCES = singleColumnFileTest.cc
#single_column_file_test_LDADD = liboobicpl.la

multiple_columns_file_test_SOURCES = multipleColumnsFileTest.cc





