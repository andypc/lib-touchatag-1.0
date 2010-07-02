#include <stdio.h>
#include <glib.h>

#include "touchatag_reader.h"
#include "touchatag_taglist.h"
#include "touchatag_time.h"

int main() {
	
	int numofdev, numtags = 0, i;
	libusb_t *libusb;
	reader_t *reader;
	tag_t *tag;
	
	libusb = g_new0 (libusb_t, 1);
	
	printf ("\n ****************\n");
	printf (" main_polling_all\n");
	printf (" ****************\n\n");
	printf (" detect all the readers and look for tags (UID and data) using lib-touchatag-1.0\n\n");
	
	if ((numofdev = touchatag_scan_bus (libusb)) != 0) {
		
		reader = g_new0 (reader_t, numofdev);
		tag = g_new0 (tag_t, 2);

		/* Set timing reader */
		touchatag_reader_set_time (5000,0);

		/* Turn on all the readers connected and send INIT commands */
		touchatag_reader_init_all (libusb, reader);

		/* Get reader IDs */
		touchatag_reader_get_all_reader_id (libusb, reader);

		for (i = 0; i < numofdev; i++)
			printf (" Reader %d | ID %s\n", i, (char *) touchatag_reader_return_reader_id (&reader[i]));
		
		/* Look for tags, get ID and DATA */
		numtags = touchatag_reader_polling_all_readers (libusb, reader, tag);
		
		if (numtags == 1) {
			char *string;
			char *string_new;
			int z = 0, j;
			string_new = (char *) malloc (22);
			
			printf ("\n Tag UID:        %s\n", (char *) touchatag_tag_return_tag_uid (&tag[0]));

			/* Get hex data */
			string = touchatag_tag_return_hex_data_mem (&tag[0]);

			/* Data fix */
			for (j = 0; j < 6; j++) {
				for (i = 0; i < 16; i++) {
					/* Create space */
					if ((i != 0) && (i % 2 == 0)) {
						string_new[z++] = ' ';
					}

					string_new[z] = string[j*16+i];
					z++;
				}
				string_new[z++] = '\0';

				if (j == 0)
					printf (" Tag HEX DATA:   %s\n", string_new);
				else
					printf ("                 %s\n", string_new);
				
				z = 0;
			}
			
			printf (" Tag ASCII DATA: %s\n\n", (char *) touchatag_tag_return_ascii_data_mem (&tag[0]));
		}
		
		else if (numtags == 2) {
			printf (" Tag UID 1: %s\n", (char *) touchatag_tag_return_tag_uid (&tag[0]));
			printf (" Tag UID 2: %s\n\n", (char *) touchatag_tag_return_tag_uid (&tag[1]));
		}

		else
			printf (" No tags found\n\n");

		touchatag_reader_power_off_all (libusb, reader);
	}
	else
		printf (" No readers found\n\n");
return 0;	
}
