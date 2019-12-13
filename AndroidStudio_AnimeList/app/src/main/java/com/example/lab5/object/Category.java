package com.example.lab5.object;

public class Category {

//    Written by:
//        Name              Zirong Xu
//        Student netID     zirongx
//        StudentID         91574614

    private String id;
    private String name;
    private int count;

    public Category(String id, String name, int count) {
        this.id = id;
        this.name = name;
        this.count = count;
    }

    public Category () {
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

    public int getCount() {
        return count;
    }

    public void setCount(int count) {
        this.count = count;
    }
}
