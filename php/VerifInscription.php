 <?php 
//Page php gerant l'inscription
//require('model.php');// recupere bibliotheque sql
require('inscriptionCheck.php');
$password= $_POST['pwd1']; // on recupere les variables en POST
//Pour le mot de passe par besoin d'ethique on le hache avant de le stocker afin d'assurer la securiter
	$nom= $_POST['lastName'];
	$prenom= $_POST['firstName'];
	$pseudo= $_POST['login'];
	$mail= $_POST['mail'];
if(isset($password)&isset($pseudo)&isset($mail)&isset($nom)&isset($prenom)){// test d'existance des variables
	$compte = new Compte($pseudo, $password,$mail,$nom,$prenom);
		if($compte->verifMail()&$compte->verifPseudo()){
	
		$password= password_hash($password, PASSWORD_DEFAULT);// hacher ethiquement
		inscription($password,$pseudo,$mail,$nom,$prenom);// Nous creeons le compte



		//TODO enrichir le document php permettant de faire des verifications et l'inserer avant inscription


		echo 'Vous etes inscrit';
		require('IndexAccueil.php');


		}
		else{
			
			if(!($compte->verifModifPseudo())){
				$errorPseudo="Le pseudo que vous avez rentre existe deja, veuillez en choisir un autre";
			}
			if(!($compte->verifMail())){
				$errorMail="Le mail que vous avez rentre n'est pas valide ou existe deja";
			}

			echo "Veuillez recommencer svp";
			require('IndexInscription.php');
		}
	}
else
{
	echo "Vous n'avez pas rempli tout les champs, Veuillez recommencer";
	require('IndexInscription.php');


}
?>
