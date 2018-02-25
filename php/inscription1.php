 <?php 
//Page php gerant l'inscription
require('model.php');// recupere bibliotheque sql
$password= password_hash($_POST['password'], PASSWORD_DEFAULT); // on recupere les variables en POST
//Pour le mot de passe par besoin d'ethique on le hache avant de le stocker afin d'assurer la securiter
	$nom= $_POST['nom'];
	$prenom= $_POST['prenom'];
	$pseudo= $_POST['pseudo'];
	$mail= $_POST['mail'];
if(isset($password)&isset($pseudo)&isset($mail)&isset($nom)&isset($prenom)){// test d'existance des variables
	

inscription($password,$pseudo,$mail,$nom,$prenom);// Nous creeons le compte
//TODO enrichir le document php permettant de faire des verifications et l'inserer avant inscription


echo 'Vous etes inscrit';



}
else{
echo "Sorry you are not ready for this adventure";
}
