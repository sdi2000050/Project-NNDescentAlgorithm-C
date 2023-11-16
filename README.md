ΕΥΑΓΓΕΛΟΥ ΓΕΩΡΓΙΑ ΑΜ: 1115202000050  sdi2000050@di.uoa.gr

ΛΟΞΟΣ ΑΝΔΡΕΟΥ ΑΛΕΞΑΝΔΡΟΣ ΑΜ: 1115202000117   sdi2000117@di.uoa.gr


Οι εργασίες διαμοιράστηκαν ως εξής:

Ευαγγέλου Γεωργία: Ανέλαβε την υλοποίηση και δημιουργία του γράφου καθώς επίσης και την επεξεργασία
των datasets.

Λοξός Ανδρέου Αλέξανδρος: Ανέλαβε την υλοποίση των αποστάσεων και το κομμάτι του testing.

Από κοινού αναλάβαμε την βασική ιδέα και υλοποίηση του αλγορίθμου nn descent.

Σχεδιαστικές επιλογές:

graph_Implemantation: Αρχικά επιλέξαμε μια δομή point στην οποία αποθηκεύουμε συντεταγμένες και διάσταση σημείου.
Στη δομή του κόμβου επιλέξαμε να αποθηκεύου το χαρακτηριστικό id του εκάστοτε κόμβου ,
τις συντεταγμένες του και δύο συνδεδεμένες λίστες κόμβων , μια για τους γείτονές του 
και μια για τους αντίστροφους γείτονές του.
Ο γράφος μας λοιπόν αποτελείται πέρα από το μέγεθος του, από έναν πίνακα από κόμβους.
Με την συνάρτηση getnodes διαβάζουμε τα δεδομένα μας από το δοσμένο αρχείο και με
την createRandomGraph αρχικοποιούμε τον γράφο μας με αυτά τα δεδομένα χρησιμοποιώντας
την συνάρτηση addEdge η οποία δημειουργεί μια ακμή μεταξύ source node και destination node
ενημερώνοντας κατάλληλα τις λίστες kneighbors και rneighbors.

nn_descent: Στο αρχείο αυτό έχουμε δημιουργήσει δύο συναρτήσεις για την εφαρμογή του αλγορίθμου.
Από την μια η nndescentpoint βρίσκει τους Κ κοντινότερους κόμβους του γράφου από δοσμένο σημείο
προσπελάβνοντας όλο το γράφο ξεκινόντας από τον δοσμένο κόμβο και ελέγχοντας σύμφωνα
με τον αλγόριθμο τους γείτονες των γειτόνων αποθηκεύοντας τους Κ κοντινότερους
σε έναν πίνακα απο KDistance δομές που περιέχουν κόμβο και απόσταση τον οποίο κρατάμε συνεχώς ταξινομημένο
σύμφωνα με την απόσταση.
Ακολουθώντας παρόμοια λογική η συνάρτηση nndescent μέχρις ότου να μην μεταβάλεται η μορφή 
του γράφου επαναλαμβάνει τα παρακάτω.
Για κάθε κόμβο πηγαίνει στους γείτονες του (κανονικούς και αντίστροφους) και ελέγχει την απόσταση του
από τους γείτονες αυτών των γειτόνων κρατώντας κάθε φορά τους Κ κοντινότερους και ενημερώνοντας τελικά τον γράφο. 

graph_main: Τέλος για την main επιλέξαμε να δίνει ο χρήστης από το command line πέρα από το αρχείο
του datasets σε μορφή unint_32 , ένα αρχείο με το επιθημητό σημείο εκτός γράφου για την εφαρμογή του 
αλγορίθμου , την διάσταση των σημείων και το Κ για τη δημιουργία του KNNG.
Στη συνέχεια δημιουργούμε έναν τυχαίο γράφο και τον εκτυπώνουμε για τον χρήστη.
Επίσης ζητείται από το χρήστη να επιλέξει την επιθημητή μετρική απόστασης και η επιθημητή
εφαρμογή του γράφου (σε ένα σημείο ή σε όλο το γράφο).
