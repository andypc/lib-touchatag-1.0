#include <stdio.h>
#include <signal.h>
#include <glib.h>

#include "touchatag_reader.h"
#include "touchatag_taglist.h"
#include "touchatag_time.h"

int main() {
	
	int numofdev, numtags = 0, i;
	char idmio[] = {'0','0','0','0','5','4'};

	libusb_t *libusb;
	reader_t *reader;
	tag_t *tag;
	notes_t notes;
	struct timeb time_data;

	
	libusb = g_new0 (libusb_t, 1);
	


	printf ("CIAO\n");
	
	if ((numofdev = touchatag_scan_bus (libusb)) !=0) {
		
		reader = g_new0 (reader_t, numofdev);
		tag = g_new0 (tag_t, 2);

		/* Set timing reader */
		touchatag_reader_set_time (5000,0);

		/* Turn on all the readers connected and send INIT commands */
		touchatag_reader_init_all (libusb, reader);

		/* Get reader IDs */
		touchatag_reader_get_all_reader_id (libusb, reader);

		printf ("Reader ID %s\n", (char *) touchatag_reader_return_reader_id (&reader[0]));
		
		/* Look for tags, get ID and DATA */
		numtags = touchatag_reader_polling_target_reader (libusb, reader, tag, idmio);

		if (numtags == 1) {
			printf ("Tag UID %s\n", (char *) touchatag_tag_return_tag_uid (&tag[0]));
		}
		
		if (numtags == 2) {
			printf ("Tag UID 1: ");
			touchatag_tag_print_tag_uid (&tag[0]);
			printf ("\n");
			printf ("Tag UID 2: ");
			touchatag_tag_print_tag_uid (&tag[1]);
			printf ("\n");
		}

		touchatag_reader_power_off_target_reader (libusb, reader, idmio);
	}
return 0;	
}
