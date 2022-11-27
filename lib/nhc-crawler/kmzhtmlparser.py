from html.parser import HTMLParser

class KmzHtmlParser(HTMLParser):
    td_count = 0
    a_count = 0
    current_row = 0
    in_table = False
    read_data = False
    reading_link = False
    links = []

    def handle_starttag(self, tag: str, attrs: list[tuple[str, str | None]]) -> None:
        if (tag == 'table'):
            self.in_table = not self.in_table
        else:
            if (self.in_table):
                if (tag == 'td' and self.current_row == 3):
                    if (self.td_count > 0 and self.td_count < 4):
                        self.read_data = True
                    self.td_count += 1
                
                if (tag == 'a' and self.read_data and self.current_row == 3):       # Don't grab the first .zip link
                    for attr in attrs:
                        if (attr[0] == 'href' and self.a_count % 4 != 0):
                            self.links.append(attr[1])
                        self.a_count += 1
               
                if (tag == 'tr'):
                    self.current_row += 1