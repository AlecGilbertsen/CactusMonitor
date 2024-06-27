<?php
// Replace with your database credentials
$servername = "192.168.56.1";
$username = "cactusTest";
$password = "cactusTest123!";
$dbname = "cactusData";

// Get humidity value from the request
$humidity = $_GET['humidity'];

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Insert humidity data into the database
$sql = "INSERT INTO humidity_data (humidity) VALUES ('$humidity')";

if ($conn->query($sql) === TRUE) {
    echo "Data stored in the database successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

// Close connection
$conn->close();
?>