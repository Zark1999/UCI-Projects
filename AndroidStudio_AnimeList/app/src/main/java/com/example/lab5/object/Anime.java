package com.example.lab5.object;

public class Anime {

//    Written by:
//        Name              Qirui Wu
//        Student netID     qiruiw3
//        StudentID         88471259

    private String id;
    private String name;
    private int rating;
    private int count;

    public Anime() {
    }

    public Anime(String id, String name, int rating, int count) {
        this.id = id;
        this.name = name;
        this.rating = rating;
        this.count = count;
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

    public int getRating() {
        return rating;
    }

    public void setRating(int rating) {
        this.rating = rating;
    }

    public int getCount() {
        return count;
    }

    public void setCount(int count) {
        this.count = count;
    }
}
