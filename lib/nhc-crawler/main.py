import os
from shutil import rmtree, copyfile

import fileoperations

from platform import system

current_platform = system()
mac_modifier = ''

if current_platform == 'Darwin':
    mac_modifier = '../../../'

if not os.path.exists(f"{mac_modifier}noaa_cache"):
        os.mkdir(f"{mac_modifier}noaa_cache")

if not os.path.exists(f"{mac_modifier}python/USE_ARCHIVE.txt"):
    fileoperations.download_file("https://www.nhc.noaa.gov/gis/", f"{mac_modifier}noaa_cache/new_nhc.html")

    if os.path.isfile(f"{mac_modifier}noaa_cache/nhc.html"):
        old_hash = fileoperations.check_hash(f"{mac_modifier}noaa_cache/nhc.html")
        new_hash = fileoperations.check_hash(f"{mac_modifier}noaa_cache/new_nhc.html")

        if old_hash == new_hash:
            os.remove(f"{mac_modifier}noaa_cache/new_nhc.html")
            print("Hashes match! No need to re-crawl.")
            raise KeyboardInterrupt
        else:
            print("Data has been updated. Now crawling.")
            os.remove(f"{mac_modifier}noaa_cache/nhc.html")
            os.rename(f"{mac_modifier}noaa_cache/new_nhc.html", "noaa_cache/nhc.html")

            if os.path.isfile(f"{mac_modifier}noaa_cache/empty.txt"):
                os.remove(f"{mac_modifier}noaa_cache/empty.txt")

    else:
        os.rename(f"{mac_modifier}noaa_cache/new_nhc.html", f"{mac_modifier}noaa_cache/nhc.html")
else:
    if os.path.isfile(f"{mac_modifier}noaa_cache/empty.txt"):
                os.remove(f"{mac_modifier}noaa_cache/empty.txt")

    copyfile(f"{mac_modifier}python/archive.html", f"{mac_modifier}noaa_cache/nhc.html")

url_obj_list = fileoperations.parse_kmz_from_html(f"{mac_modifier}noaa_cache/nhc.html")

# Clean output dirs from cache directory
listdir_output = os.listdir(f"{mac_modifier}noaa_cache")
dirs_to_remove = []

for item in listdir_output:
    if os.path.isdir(f"{mac_modifier}noaa_cache/{item}"):
        dirs_to_remove.append(item)

for dir in dirs_to_remove:
    rmtree(f"{mac_modifier}noaa_cache/{dir}")

# Len of an empty dict is 0
if (len(url_obj_list[0]) == 0):
    print("No avaliable kmz files!")
    with open(f"{mac_modifier}noaa_cache/empty.txt", "w") as empty_file:
        empty_file.write("NO AVALIABLE HURRICANE DATA!")
    raise KeyboardInterrupt

# Download files into i directories
i = 0
for url_obj in url_obj_list:
    os.mkdir(f"{mac_modifier}noaa_cache/{i}")

    url = url_obj["cone"]
    full_path = f"{mac_modifier}noaa_cache/" + str(i) + "/cone.kmz"
    fileoperations.download_file(url, full_path)
    cone_dir = f"{mac_modifier}noaa_cache/{i}/cone"
    os.mkdir(cone_dir)
    fileoperations.unzip_file(full_path, cone_dir)

    url = url_obj["track"]
    full_path = f"{mac_modifier}noaa_cache/" + str(i) + "/track.kmz"
    fileoperations.download_file(url, full_path)
    track_dir = f"{mac_modifier}noaa_cache/{i}/track"
    os.mkdir(track_dir)
    fileoperations.unzip_file(full_path, track_dir)
  
    url = url_obj["warnings"]
    full_path = f"{mac_modifier}noaa_cache/" + str(i) + "/warnings.kmz"
    fileoperations.download_file(url, full_path)
    warning_dir = f"{mac_modifier}noaa_cache/{i}/warnings"
    os.mkdir(warning_dir)
    fileoperations.unzip_file(full_path, warning_dir)

    i += 1