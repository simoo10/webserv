#!/usr/bin/php-cgi

<?php
// Set the Content-Type header to indicate that the response is HTML
header("Content-Type: text/html");

// Print the HTML content
echo "<!DOCTYPE html>\n";
echo "<html>\n";
echo "<head>\n";
echo "<title>Hello, CGI PHP!</title>\n";
echo "</head>\n";
echo "<body>\n";
echo "<h1>Hello, CGI PHP!</h1>\n";
echo "<p>This is a test CGI PHP script.</p>\n";
echo "</body>\n";
echo "</html>\n";
?>