package com.example.lab5.object;

public class Episode {

//    Written by:
//        Name              Zhenghao Li
//        Student netID     zhenghl3
//        StudentID         65553969

    private String id;
    private String name;
    private String description;
    private String link;

    public Episode() {
    }

    public Episode(String id, String name, String link, String description) {
        this.id = id;
        this.name = name;
        this.description = description;
        this.link = link;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String getLink() {
        return link;
    }

    public void setLink(String link) {
        this.link = link;
    }
}
