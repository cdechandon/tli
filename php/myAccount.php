<?php
// page php qui en fonction de l'existance d'une session ou non va rediriger l'utilisateur
session_start();
if (isset($_SESSION['id']) AND isset($_SESSION['pseudo']))

{

    require('IndexMyAccount.php');

}
else {
	require('RedirectionMyAccount.php');
}

?>
