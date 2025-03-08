#ifndef PUBLISH_CSV_H
#define PUBLISH_CSV_H

void open_csv_file(const char *filename);
void append_csv_data(int aqi, int tvoc, int eco2);
void close_csv_file();

#endif // PUBLISH_CSV_H