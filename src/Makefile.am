
bin_PROGRAMS = vertstatsinfo \
	vertstats_extract \
	vertstats_colour_object \
	vertstats_math

include_HEADERS = mniVolume.h \
	mniLabelVolume.h \
	mniBaseVolume.h \
	mniVertstatsFile.h \
	mniVertstatsMath.h \
	tree.hh 

LDADD = liboobicpl.la

lib_LTLIBRARIES = liboobicpl.la

TESTS = vertstats_tests \
	single_column_file_test \
	multiple_columns_file_test

check_PROGRAMS = $(TESTS)

liboobicpl_la_SOURCES = mniBaseVolume.cc \
	mniVolume.cc \
	mniLabelVolume.cc \
	mniVertstatsFile.cc \
	mniVertstatsMath.cc

# Programmes
vertstatsinfo_SOURCES = vertstatsinfo.cc
vertstats_math_SOURCES = vertstats_math.cc

vertstats_extract_SOURCES = vertstatsExtract.cc

vertstats_colour_object_SOURCES = vertstats_colour_object.cc


# Tests
#negative_values_test_SOURCES = negativeValuesTest.cc
#volume_tests_SOURCES = volumeTests.cc
vertstats_tests_SOURCES = vertstatsTests.cc
single_column_file_test_SOURCES = singleColumnFileTest.cc
multiple_columns_file_test_SOURCES = multipleColumnsFileTest.cc





