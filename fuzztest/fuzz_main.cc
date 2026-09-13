#include "absl/debugging/failure_signal_handler.h"
#include "absl/debugging/symbolize.h"
#include "fuzztest/init_fuzztest.h"
#include "gtest/gtest.h"

#include <QApplication>

int main(int argc, char **argv)
{
    absl::InitializeSymbolizer(argv[0]);
    absl::FailureSignalHandlerOptions options;
    options.call_previous_handler = true;
    absl::InstallFailureSignalHandler(options);

    testing::InitGoogleTest(&argc, argv);
    fuzztest::ParseAbslFlags(argc, argv);
    fuzztest::InitFuzzTest(&argc, &argv);

    // Qt is needed for QPixmap members, including tests without visible windows.
    QApplication application(argc, argv);
    return RUN_ALL_TESTS();
}
