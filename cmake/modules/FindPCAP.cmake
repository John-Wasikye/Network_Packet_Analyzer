# Locate PCAP library and include files
#   PCAP_INCLUDE_DIRS - where to find pcap.h, etc.
#   PCAP_LIBRARIES    - List of libraries when using PCAP.
#   PCAP_FOUND        - True if PCAP found.

find_path(PCAP_INCLUDE_DIR pcap.h)

if (WIN32)
    find_library(PCAP_LIBRARY wpcap)
else ()
    find_library(PCAP_LIBRARY pcap)
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PCAP DEFAULT_MSG PCAP_LIBRARY PCAP_INCLUDE_DIR)

mark_as_advanced(PCAP_INCLUDE_DIR PCAP_LIBRARY)

if (PCAP_FOUND)
    set(PCAP_LIBRARIES ${PCAP_LIBRARY})
endif ()
