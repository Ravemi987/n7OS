.text
# cette directive sert a rendre l'etiquette publique
    .globl handler_IT20
# debut du traitant
handler_IT20:
# sauvegarde des registres importants
    pushl %eax
    pushl %edx
    pushl %ecx
# appel a la fonction C realisant le traitant
    call handler_it20
# restauration des registres importants
    popl %ecx
    popl %edx
    popl %eax
# fin du traitant
    iret
