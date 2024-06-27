<?php
// Replace with your database credentials
$servername = "192.168.56.1";
$username = "cactusTest";
$password = "cactusTest123!";
$dbname = "cactusData"; // Use the name of the database you created

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Fetch humidity data from the database
$sql = "SELECT humidity FROM humidity_data";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // Convert the result set to JSON format
    $data = array();
    while ($row = $result->fetch_assoc()) {
        $data[] = $row;
    }
    echo json_encode($data);
} else {
    echo "No data found";
}

// Close connection
$conn->close();
?>