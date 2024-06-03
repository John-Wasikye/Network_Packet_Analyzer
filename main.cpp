#include <pcap.h>
#include <iostream>
#include <ncurses.h>
#include <signal.h>

volatile sig_atomic_t keep_running = 1;

void int_handler(int dummy) {
    keep_running = 0;
}

void packet_handler(u_char *user, const struct pcap_pkthdr *header, const u_char *packet) {
    clear();
    mvprintw(0, 0, "Captured a packet with length of [%d]", header->len);
    refresh();
}

int main() {
    pcap_if_t *alldevs;
    pcap_if_t *device;
    char errbuf[PCAP_ERRBUF_SIZE];

    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);

    // Find all network devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        printw("Error finding devices: %s\n", errbuf);
        refresh();
        endwin();
        return 1;
    }

    // Print the list of devices
    int row = 0;
    for (device = alldevs; device; device = device->next, row++) {
        mvprintw(row, 0, "%s - %s\n", device->name, device->description ? device->description : "No description available");
    }
    refresh();

    // Open the first device for packet capture
    pcap_t *handle = pcap_open_live(alldevs->name, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        mvprintw(row + 1, 0, "Could not open device: %s\n", errbuf);
        refresh();
        pcap_freealldevs(alldevs);
        endwin();
        return 2;
    }

    // Set the signal handler
    signal(SIGINT, int_handler);

    // Start packet capture
    pcap_loop(handle, 0, packet_handler, NULL);

    // Clean up
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // End ncurses mode
    endwin();
    return 0;
}
