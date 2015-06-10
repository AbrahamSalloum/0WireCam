<html>
<head>
<title>SENSORS</title>
</head><body bgcolor = "silver">
<?php
$connection = mysql_connect('localhost', 'root', 'salloum1'); //The Blank string is the password
mysql_select_db('testdb');

$query = "SELECT * FROM sensors";
$result = mysql_query($query);
echo "<h4>SUMMARY</h4>"; 
echo "<table border = '1'>";
echo "<tr><td bgcolor=white><h4>ID</h4></td><td bgcolor=white><h4>State</h4></td></tr>"; 
while($row = mysql_fetch_array($result)){
echo "<tr><td bgcolor=white>" . $row['id'] . "</td>";
echo "<td"; 
if($row['state'] == 1){
echo " bgcolor=pink"; 
} 
if($row['state'] == 0){
echo " bgcolor=aqua"; 
}
echo ">" . $row['state'] . "</td></tr>";
}
echo "</table>";
# detiled data starts here
echo "<hr size=4>";
echo "<center>"; 
echo "<h1> Detailed View </h1>";  
$result = mysql_query($query);
while($row = mysql_fetch_array($result)){
echo "FOR SENSOR WITH <b>ID</b> VALUE: <b>" . $row['id'] . " </b><br>"; 
if ($row['state'] == 0){
echo "<div style="; echo "border:solid;width:90%;height:200px;background-color:pink;"; echo ">"; 
echo "MOTION HAS BEEN DETECTED <br>"; 
echo "<form method='post'>";
echo "switch:"; 
echo "<input type='submit' name='on' id ='on' value='on'>";
echo "<input type='submit' name='off' id ='off' value='off'>";
echo "<br>&nbsp;&nbsp;  ";
echo "<input type='submit' name='pic' id ='pic' value='pic'>";
echo "<br>";
echo "<input type='submit' name='dismiss' id ='dismiss' value='dismiss'>";
echo "</form>";

if(isset($_POST['on'])){
echo "The switch has been turned on, press off to power down"; 
$SQL = "UPDATE switch SET switch = '1' WHERE id=".$row[id]; 
$result = mysql_query($SQL);
}


if(isset($_POST['pic'])){
echo "A picture has been taken. "; 
$SQL = "UPDATE cam SET cam = '2' WHERE id=".$row[id]; 
$result = mysql_query($SQL);
}

if(isset($_POST['off'])){
echo "The switch has been turned off, press on to power up"; 
$SQL = "UPDATE switch SET switch = '0' WHERE id=".$row[id]; 
$result = mysql_query($SQL);
}

if(isset($_POST['dismiss'])){
echo "The sensor has been reset"; 
$SQL = "UPDATE sensors SET state = '0' WHERE id=".$row[id]; 
$result = mysql_query($SQL);
}

$link = "./".$row[id]."/img/";
echo "<a href='".$link."'>View Pictures</a>";
echo "</div>";
echo "<br>"; 
}
if ($row['state'] == 0){
echo "<div style="; echo "border:solid;width:90%;height:200px;background-color:aqua;"; echo ">"; 
echo "MOTION HAS <b><i>NOT</b></i> BEEN DETECTED <br>"; 

$link = "./".$row[id]."/img/";
echo "<a href='".$link."'>View Pictures</a>";
echo "</div>"; 
}
echo "<hr size=4>";
}
mysql_close();
echo "<br>"
?>

</center></body></html>
