#!/usr/bin/python

import xml.etree.ElementTree as ET

#file = open('index.xml')

tree = ET.parse('index.xml')

root = tree.getroot()
'''
the index.xml is mainly as below format:
<error>
<checker>BAD_ALLOC_ARITHMETIC</checker>
<file>/home/zdu/data/ga_root/makefile_base/uci/list.c</file>
<function>uci_alloc_package</function>
<unmangled_function>uci_alloc_package</unmangled_function>
<status>Unclassified</status>
<num>1</num>
<home>1/1list.c</home>
</error>


we only need qwcfg, wifi_hal errors
find the matched error, then save the files to out.xml
'''
num=0
for item in root.findall('error'):
    file1 = item.find('file').text
    if 'qwcfg' in file1 or 'wifi_hal' in file1 :
        print file1
        num += 1
    else:
        root.remove(item)
print num
tree.write('out.xml')
