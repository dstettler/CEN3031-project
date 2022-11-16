import fileoperations
import os
from shutil import rmtree

if not os.path.exists("noaa_cache"):
        os.mkdir("noaa_cache")

fileoperations.download_file("https://www.nhc.noaa.gov/gis/", "noaa_cache/new_nhc.html")

if os.path.isfile("noaa_cache/nhc.html"):
    old_hash = fileoperations.check_hash("noaa_cache/nhc.html")
    new_hash = fileoperations.check_hash("noaa_cache/new_nhc.html")

    if old_hash == new_hash:
        os.remove("noaa_cache/new_nhc.html")
        print("Hashes match! No need to re-crawl.")
        quit()
    else:
        print("Data has been updated. Now crawling.")
        os.remove("noaa_cache/nhc.html")
        os.rename("noaa_cache/new_nhc.html", "noaa_cache/nhc.html")

        if os.path.isfile("noaa_cache/empty.txt"):
            os.remove("noaa_cache/empty.txt")

else:
    os.rename("noaa_cache/new_nhc.html", "noaa_cache/nhc.html")


url_obj_list = fileoperations.parse_kmz_from_html("noaa_cache/nhc.html")

# Clean output dirs from cache directory
listdir_output = os.listdir("noaa_cache")
dirs_to_remove = []

for item in listdir_output:
    if os.path.isdir(f"noaa_cache/{item}"):
        dirs_to_remove.append(item)

for dir in dirs_to_remove:
    rmtree(f"noaa_cache/{dir}")

# Len of an empty dict is 0
if (len(url_obj_list[0]) == 0):
    print("No avaliable kmz files!")
    with open("noaa_cache/empty.txt", "w") as empty_file:
        empty_file.write("NO AVALIABLE HURRICANE DATA!")
    quit()

# Download files into i directories
i = 0
for url_obj in url_obj_list:
    os.mkdir(f"noaa_cache/{i}")

    url = url_obj["cone"]
    full_path = "noaa_cache/" + str(i) + "/cone.kmz"
    fileoperations.download_file(url, full_path)
    cone_dir = f"noaa_cache/{i}/cone"
    os.mkdir(cone_dir)
    fileoperations.unzip_file(full_path, cone_dir)

    url = url_obj["track"]
    full_path = "noaa_cache/" + str(i) + "/track.kmz"
    fileoperations.download_file(url, full_path)
    track_dir = f"noaa_cache/{i}/track"
    os.mkdir(track_dir)
    fileoperations.unzip_file(full_path, track_dir)
  
    url = url_obj["warnings"]
    full_path = "noaa_cache/" + str(i) + "/warnings.kmz"
    fileoperations.download_file(url, full_path)
    warning_dir = f"noaa_cache/{i}/warnings"
    os.mkdir(warning_dir)
    fileoperations.unzip_file(full_path, warning_dir)

    i += 1