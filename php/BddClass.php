<?php
//Pas fini pour faire un model en POO
class Bdd
{
    private $_basis;
    private $_table;
    private $_sgbd;


    function __construct($sgbd,$basis, $table)
    {
        $this->_basis = $basis;
	$this->_table = $table;
	$this->_sgbd = $sgbd;

    }

	public function connexionToBdd(){
try{
$bdd = new PDO('mysql:host=localhost;dbname=TLI;charset=utf8', 'root', '');
$bdd =new PDO(’mysql:host=localhost;dbname=’.$this->dbName, $this->user, $this->pass,
array
(
PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8"));
}
catch(Exception $e)
{
        die('Erreur : '.$e->getMessage());
}
return $bdd;	

}





?>
