//------------------------------------------------------------------------------
// <auto-generated>
//     Ce code a été généré à partir d'un modèle.
//
//     Des modifications manuelles apportées à ce fichier peuvent conduire à un comportement inattendu de votre application.
//     Les modifications manuelles apportées à ce fichier sont remplacées si le code est régénéré.
// </auto-generated>
//------------------------------------------------------------------------------

namespace DbStatStore
{
    using System;
    using System.Collections.Generic;
    
    public partial class DbDataset
    {
        public DbDataset()
        {
            this.Indivs = new HashSet<DbIndiv>();
            this.Variables = new HashSet<DbVariable>();
        }
    
        public int Id { get; set; }
        public int Version { get; set; }
        public string Sigle { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
    
        public virtual ICollection<DbIndiv> Indivs { get; set; }
        public virtual ICollection<DbVariable> Variables { get; set; }
    }
}