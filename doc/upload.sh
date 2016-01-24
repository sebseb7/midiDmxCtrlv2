{ echo -ne "HTTP/1.0 200 OK\r\nContent-Type: application/touchsc\r\nContent-Disposition:attachment; filename:\"remote.touchosc\"\r\n\r\n"; zcat remote.touchosc; } |  nc -l -p 9658
