<?php
//Page php permettant de detruire la session et donc de se deconnecter
session_start();
session_destroy();
require('IndexAccueil.php');
?>
