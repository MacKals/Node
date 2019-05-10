#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed May  8 21:13:56 2019

@author: mkals
"""

import http.server
import socketserver

PORT = 8080
Handler = http.server.SimpleHTTPRequestHandler
with socketserver.TCPServer(("", PORT), Handler) as httpd:
    print("Serving at port", PORT)
    httpd.serve_forever()