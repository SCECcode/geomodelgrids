/**
 * C++ unit testing of geomodelgrids::apps::TestQuery.
 */

#include <portinfo>

#include "geomodelgrids/apps/Query.hh" // USES Query
#include "geomodelgrids/utils/constants.hh" // USES NODATA_VALUE

#include "ModelPoints.hh"

#include <cppunit/extensions/HelperMacros.h>

#include <fstream> // USES std::ifstream, std::ofstream
#include <iomanip> // USES ios::setf(), ios::setprecision()
#include <getopt.h> // USES optind
#include <cmath> // USES fabs()

namespace geomodelgrids {
    namespace apps {
        class TestQuery;
    } // apps
} // geomodelgrids

class geomodelgrids::apps::TestQuery : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE /////////////////////////////////////////////////
    CPPUNIT_TEST_SUITE(TestQuery);

    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testParseArgsHelp);
    CPPUNIT_TEST(testParseArgsNoPoints);
    CPPUNIT_TEST(testParseArgsNoModels);
    CPPUNIT_TEST(testParseArgsNoOutput);
    CPPUNIT_TEST(testParseArgsWrong);
    CPPUNIT_TEST(testParseArgsMinimal);
    CPPUNIT_TEST(testParseArgsAll);
    CPPUNIT_TEST(testPrintHelp);
    CPPUNIT_TEST(testRunHelp);
    CPPUNIT_TEST(testRunOneBlockFlat);
    CPPUNIT_TEST(testRunThreeBlocksTopo);
    CPPUNIT_TEST(testRunThreeBlocksSquash);
    CPPUNIT_TEST(testRunTwoModels);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

    /// Setup test.
    void setUp(void);

    /// Test constructor.
    void testConstructor(void);

    /// Test _parseArgs() with --help.
    void testParseArgsHelp(void);

    /// Test _parseArgs() missing --points.
    void testParseArgsNoPoints(void);

    /// Test _parseArgs() missing --models.
    void testParseArgsNoModels(void);

    /// Test _parseArgs() missing --output.
    void testParseArgsNoOutput(void);

    /// Test _parseArgs() with wrong arguments.
    void testParseArgsWrong(void);

    /// Test _parseArgs() with required arguments.
    void testParseArgsMinimal(void);

    /// Test _parseArgs() with all arguments.
    void testParseArgsAll(void);

    /// Test _printHelp().
    void testPrintHelp(void);

    /// Test run() wth help.
    void testRunHelp(void);

    /// Test run() wth one-block-flat.
    void testRunOneBlockFlat(void);

    /// Test run() wth three-blocks-topo.
    void testRunThreeBlocksTopo(void);

    /// Test run() wth three-blocks-topo with squashing.
    void testRunThreeBlocksSquash(void);

    /// Test run() wth one-block-flat and three-blocks-topo.
    void testRunTwoModels(void);

}; // class TestQuery
CPPUNIT_TEST_SUITE_REGISTRATION(geomodelgrids::apps::TestQuery);

// ---------------------------------------------------------------------------------------------------------------------
namespace geomodelgrids {
    namespace apps {
        class _TestQuery;
    } // apps
} // geomodelgrids

class geomodelgrids::apps::_TestQuery {
public:

    static
    void createPointsFile(std::ostream& sout,
                          const geomodelgrids::testdata::ModelPoints& points);

    static
    void checkQuery(std::istream& sin,
                    const geomodelgrids::testdata::ModelPoints& points);

}; // _TestQuery

// ---------------------------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::TestQuery::setUp(void) {
    optind = 1; // reset parsing of argc and argv
} // setUp


// ---------------------------------------------------------------------------------------------------------------------
// Test constructor.
void
geomodelgrids::apps::TestQuery::testConstructor(void) {
    Query query;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in points CRS.", std::string("EPSG:4326"), query._pointsCRS);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in squash minimum elevation.", 0.0, query._squashMinElev);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Squash flag.", false, query._squash);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in help flag.", false, query._showHelp);
} // testConstructor


// ---------------------------------------------------------------------------------------------------------------------
// Test _parseArgs() with --help.
void
geomodelgrids::apps::TestQuery::testParseArgsHelp(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--help" };

    Query query;
    query._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_MESSAGE("Mismatch in help.", query._showHelp);
} // testParseArgsHelp


// ---------------------------------------------------------------------------------------------------------------------
// Test _parseArgs() without --points.
void
geomodelgrids::apps::TestQuery::testParseArgsNoPoints(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--models=A" };

    Query query;
    CPPUNIT_ASSERT_THROW(query._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoPoints


// ---------------------------------------------------------------------------------------------------------------------
// Test _parseArgs() without --models.
void
geomodelgrids::apps::TestQuery::testParseArgsNoModels(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--points=A" };

    Query query;
    CPPUNIT_ASSERT_THROW(query._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoModels


// ---------------------------------------------------------------------------------------------------------------------
// Test _parseArgs() without --output.
void
geomodelgrids::apps::TestQuery::testParseArgsNoOutput(void) {
    const int nargs = 3;
    const char* const args[nargs] = { "test", "--points=A", "--models=B" };

    Query query;
    CPPUNIT_ASSERT_THROW(query._parseArgs(nargs, const_cast<char**>(args)), std::runtime_error);
} // testParseArgsNoOutput


// ---------------------------------------------------------------------------------------------------------------------
// Test _parseArgs() with wrong arguments.
void
geomodelgrids::apps::TestQuery::testParseArgsWrong(void) {
    const int nargs = 2;
    const char* const args[nargs] = { "test", "--blah" };

    Query query;
    CPPUNIT_ASSERT_THROW(query._parseArgs(nargs, const_cast<char**>(args)), std::logic_error);
} // testParseArgsWrong


// ---------------------------------------------------------------------------------------------------------------------
// Test _parseArgs() with required arguments.
void
geomodelgrids::apps::TestQuery::testParseArgsMinimal(void) {
    const int nargs = 4;
    const char* const args[nargs] = {
        "test",
        "--models=A",
        "--points=points.in",
        "--output=points.out",
    };

    Query query;
    query._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of models.", size_t(1), query._modelFilenames.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in models.", std::string("A"), query._modelFilenames[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in points.", std::string("points.in"), query._pointsFilename);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in output.", std::string("points.out"), query._outputFilename);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in help for minimal args.", !query._showHelp);
} // testParseArgsMinimal


// ---------------------------------------------------------------------------------------------------------------------
// Test _parseArgs() with all arguments.
void
geomodelgrids::apps::TestQuery::testParseArgsAll(void) {
    const int nargs = 8;
    const char* const args[nargs] = {
        "test",
        "--models=A",
        "--points=points.in",
        "--output=points.out",
        "--points-coordsys=EPSG:26910",
        "--squash-min-elev=-2.0e+3",
        "--values=one,two,three",
        "--log=error.log",
    };
    const size_t numValues = 3;
    const char* const valueNamesE[numValues] = { "one", "two", "three" };

    Query query;
    query._parseArgs(nargs, const_cast<char**>(args));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of models.", size_t(1), query._modelFilenames.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in models.", std::string("A"), query._modelFilenames[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in points.", std::string("points.in"), query._pointsFilename);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in output.", std::string("points.out"), query._outputFilename);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in points coordsys.", std::string("EPSG:26910"), query._pointsCRS);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in squash min elevation.", -2.0e+3, query._squashMinElev);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in number of values.", numValues, query._valueNames.size());
    for (size_t i = 0; i < numValues; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in value name.", std::string(valueNamesE[i]), query._valueNames[i]);
    } // for
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mismatch in log.", std::string("error.log"), query._logFilename);
    CPPUNIT_ASSERT_MESSAGE("Mismatch in help.", !query._showHelp);
} // testParseArgsAll


// ---------------------------------------------------------------------------------------------------------------------
// Test _printHelp().
void
geomodelgrids::apps::TestQuery::testPrintHelp(void) {
    std::streambuf* coutOrig = std::cout.rdbuf();
    std::ostringstream coutHelp;
    std::cout.rdbuf(coutHelp.rdbuf() );

    Query query;
    query._printHelp();
    std::cout.rdbuf(coutOrig);
    CPPUNIT_ASSERT_EQUAL(size_t(860), coutHelp.str().length());
} // testPrintHelp


// ----------------------------------------------------------------------
// Test run() with help.
void
geomodelgrids::apps::TestQuery::testRunHelp(void) {
    std::streambuf* coutOrig = std::cout.rdbuf();
    std::ostringstream coutHelp;
    std::cout.rdbuf(coutHelp.rdbuf() );

    Query query;
    const int nargs = 2;
    const char* const args[nargs] = {
        "test",
        "--help",
    };
    query.run(nargs, const_cast<char**>(args));

    std::cout.rdbuf(coutOrig);
    CPPUNIT_ASSERT_EQUAL(size_t(860), coutHelp.str().length());
} // testRunHelp


// ---------------------------------------------------------------------------------------------------------------------
// Test run() with one-block-flat.
void
geomodelgrids::apps::TestQuery::testRunOneBlockFlat(void) {
    const int nargs = 7;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/one-block-flat.h5",
        "--points=one-block-flat.in",
        "--output=one-block-flat.out",
        "--points-coordsys=EPSG:4326",
        "--values=two,one",
        "--log=error.log",
    };
    geomodelgrids::testdata::OneBlockFlatPoints pointsOne;
    std::ofstream sout("one-block-flat.in");CPPUNIT_ASSERT(sout.is_open() && sout.good());
    _TestQuery::createPointsFile(sout, pointsOne);
    sout.close();

    Query query;
    query.run(nargs, const_cast<char**>(args));

    std::ifstream sin("one-block-flat.out");CPPUNIT_ASSERT(sin.is_open() && sin.good());
    _TestQuery::checkQuery(sin, pointsOne);
    sin.close();

    std::ifstream slog("error.log");CPPUNIT_ASSERT(slog.is_open() && slog.good());
} // testRunOneBlockFlat


// ---------------------------------------------------------------------------------------------------------------------
// Test run() with three-blocks-topo.
void
geomodelgrids::apps::TestQuery::testRunThreeBlocksTopo(void) {
    const int nargs = 6;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/three-blocks-topo.h5",
        "--points=three-blocks-topo.in",
        "--output=three-blocks-topo.out",
        "--points-coordsys=EPSG:4326",
        "--values=two,one",
    };
    geomodelgrids::testdata::ThreeBlocksTopoPoints pointsThree;
    std::ofstream sout("three-blocks-topo.in");CPPUNIT_ASSERT(sout.is_open() && sout.good());
    _TestQuery::createPointsFile(sout, pointsThree);
    sout.close();

    Query query;
    query.run(nargs, const_cast<char**>(args));

    std::ifstream sin("three-blocks-topo.out");CPPUNIT_ASSERT(sin.is_open() && sin.good());
    _TestQuery::checkQuery(sin, pointsThree);
    sin.close();
} // testRunThreeBlocksTopo


// ---------------------------------------------------------------------------------------------------------------------
// Test run() with three-blocks-topo with squashing.
void
geomodelgrids::apps::TestQuery::testRunThreeBlocksSquash(void) {
    const int nargs = 7;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/three-blocks-topo.h5",
        "--points=three-blocks-topo.in",
        "--output=three-blocks-topo.out",
        "--points-coordsys=EPSG:4326",
        "--values=two,one",
        "--squash-min-elev=-3.0e+3"
    };
    geomodelgrids::testdata::ThreeBlocksSquashPoints pointsThree(-3.0e+3);
    std::ofstream sout("three-blocks-topo.in");CPPUNIT_ASSERT(sout.is_open() && sout.good());
    _TestQuery::createPointsFile(sout, pointsThree);
    sout.close();

    Query query;
    query.run(nargs, const_cast<char**>(args));

    std::ifstream sin("three-blocks-topo.out");CPPUNIT_ASSERT(sin.is_open() && sin.good());
    _TestQuery::checkQuery(sin, pointsThree);
    sin.close();
} // testRunThreeBlocksSquash


// ---------------------------------------------------------------------------------------------------------------------
// Test run() with one-block-flat and three-blocks-topo.
void
geomodelgrids::apps::TestQuery::testRunTwoModels(void) {
    const int nargs = 6;
    const char* const args[nargs] = {
        "test",
        "--models=../../data/one-block-flat.h5,../../data/three-blocks-topo.h5",
        "--points=two-models.in",
        "--output=two-models.out",
        "--points-coordsys=EPSG:4326",
        "--values=two,one",
    };
    geomodelgrids::testdata::OneBlockFlatPoints pointsOne;
    geomodelgrids::testdata::ThreeBlocksTopoPoints pointsThree;
    std::ofstream sout("two-models.in");CPPUNIT_ASSERT(sout.is_open() && sout.good());
    _TestQuery::createPointsFile(sout, pointsOne);
    _TestQuery::createPointsFile(sout, pointsThree);
    sout.close();

    Query query;
    query.run(nargs, const_cast<char**>(args));

    std::ifstream sin("two-models.out");CPPUNIT_ASSERT(sin.is_open() && sin.good());
    _TestQuery::checkQuery(sin, pointsOne);
    _TestQuery::checkQuery(sin, pointsThree);
    sin.close();
} // testRunTwoModels


// ---------------------------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::_TestQuery::createPointsFile(std::ostream& sout,
                                                  const geomodelgrids::testdata::ModelPoints& points) {
    const size_t spaceDim = 3;
    const size_t numPoints = points.getNumPoints();
    const double* const pointsLLE = points.getLatLonElev();

    sout << std::fixed << std::setprecision(8);
    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        for (size_t iDim = 0; iDim < spaceDim; ++iDim) {
            sout << std::setw(15) << pointsLLE[iPt*spaceDim+iDim];
        } // for
        sout << "\n";
    } // for
} // createPointsFile


// ---------------------------------------------------------------------------------------------------------------------
void
geomodelgrids::apps::_TestQuery::checkQuery(std::istream& sin,
                                            const geomodelgrids::testdata::ModelPoints& points) {
    const size_t spaceDim = 3;
    const size_t numPoints = points.getNumPoints();
    const double* const pointsXYZ = points.getXYZ();
    const double* const pointsLLE = points.getLatLonElev();

    for (size_t iPt = 0; iPt < numPoints; ++iPt) {
        const double x = pointsXYZ[iPt*spaceDim+0];
        const double y = pointsXYZ[iPt*spaceDim+1];
        const double z = pointsXYZ[iPt*spaceDim+2];

        double xyz[spaceDim];
        for (size_t iDim = 0; iDim < spaceDim; ++iDim) {
            sin >> xyz[iDim];
        } // for

        { // Value 'two'
            double value = NODATA_VALUE;
            sin >> value;

            const double valueE = points.computeValueTwo(x, y, z);

            std::ostringstream msg;
            msg << "Mismatch for value 'two' for point ("
                << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ", " << pointsLLE[iPt*spaceDim+2] << ").";
            const double tolerance = 1.0e-6;
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, value, valueTolerance);
        } // Value 'two'

        { // Value 'one'
            double value = NODATA_VALUE;
            sin >> value;

            const double valueE = points.computeValueOne(x, y, z);

            std::ostringstream msg;
            msg << "Mismatch for value 'one' for point ("
                << pointsLLE[iPt*spaceDim+0] << ", " << pointsLLE[iPt*spaceDim+1] << ", " << pointsLLE[iPt*spaceDim+2] << ").";
            const double tolerance = 1.0e-6;
            const double valueTolerance = std::max(tolerance, tolerance*fabs(valueE));
            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str().c_str(), valueE, value, valueTolerance);
        } // Value 'one'
    } // for
} // checkQuery


// End of file