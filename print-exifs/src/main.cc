#include <iostream>
#include <libexif/exif-data.h>
#include <libexif/exif-loader.h>

int main(int argc, const char** argv) {
    if (argc != 2) {
        std::cout << "Please provide exactly one path to a file with EXIF data\n";
        return -1;
    }

    const char* pFilePath = argv[1];

    ExifLoader* l = exif_loader_new();
    exif_loader_write_file(l, pFilePath);

    ExifData* ed = exif_loader_get_data(l);

    for (unsigned tagId = 0; tagId < 0xffff; ++tagId) {
        /*
         * Display the name of the first tag of this number found.
         * Since there is some overlap (e.g. with GPS tags), this
         * name could sometimes be incorrect for the specific tags
         * found in this file.
         */
        ExifTag tag = (ExifTag)tagId;
        const char* name = exif_tag_get_title(tag);
        if (!name)
            continue;

        std::cout << "Tag title='" << name << "', values:";
        for (int i = (ExifIfd)0; i < EXIF_IFD_COUNT; i++) {
            ExifEntry* pEntry = exif_content_get_entry(ed->ifd[i], tag);

            if (pEntry) {
                char buffer[1024];
                exif_entry_get_value(pEntry, buffer, sizeof buffer);
                std::cout << buffer << ", ";
            }
            else {
                std::cout << "not found,";
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';

    exif_data_unref(ed);
    exif_loader_unref(l);

    return 0;
}