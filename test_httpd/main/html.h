string index_html(R"html(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32</title>
  <meta charset="utf-8">
</head>
<body>
  <table border="1">
    <tr>
      <th>Parameter</th><th>Value</th>
    </tr>
    <tr>
      <td>Video mode</td><td>{videoMode}</td>
    </tr>
  </table>
</body>
</html>
)html");
