import urllib.request
import hashlib
import html.parser

from zipfile import ZipFile

import kmzhtmlparser

from typing import Dict, List

def download_file(url: str, filepath: str) -> None:
    print(f"Downloading file {url} to {filepath}")
    remote_file = urllib.request.urlopen(url)
    html_file = open(filepath, 'wb')
    html_file.write(remote_file.read())
    html_file.close()

def check_hash(path: str) -> str:
    with open(path, 'rb') as f:
        bytes = f.read()
        hash = hashlib.sha256(bytes).hexdigest()
        print(f"SHA256 of {path} is: {hash}")
        return hash

def unzip_file(filepath: str, extractpath: str) -> None:
    with ZipFile(filepath, 'r') as zip_object:
        zip_object.extractall(extractpath)

def parse_kmz_from_html(filepath: str) -> List[Dict[str, str]]:
    parser = kmzhtmlparser.KmzHtmlParser()
    with open(filepath, 'r') as f:
        parser.feed(f.read())

        urls = []
        obj = {}
        i = 0
        
        for link in parser.links:
            if (i % 3 == 0 and i > 0):
                urls.append(obj)
                obj.clear()

            if (link.find("CONE") != -1):
                obj["cone"] = link
            elif (link.find("TRACK") != -1):
                obj["track"] = link
            elif (link.find("WW") != -1):
                obj["warnings"] = link

            i += 1
        
        urls.append(obj)

    return urls