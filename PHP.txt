<?php
$temp = filter_input(INPUT_GET, 'temp', FILTER_SANITIZE_NUMBER_FLOAT);
$humid = filter_input(INPUT_GET, 'humid', FILTER_SANITIZE_NUMBER_FLOAT);
if (is_null($temp) || is_null($humid) ) {
  //Gravar log de erros
  die("Dados inválidos");
} 
$servername = "localhost";
$username = "root";
$password = "root";
$dbname = "maker";
$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
  //Gravar log de erros
  die("Não foi possível estabelecer conexão com o BD: " . $conn->connect_error);
} 
$sql = "INSERT INTO weather (wea_temp, wea_humid) VALUES ($temp,$humid)";
 
if (!$conn->query($sql)) {
  //Gravar log de erros
  die("Erro na gravação dos dados no BD");
}
$conn->close();
?>