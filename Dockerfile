# Use a lightweight GCC image
FROM gcc:latest

# Install CMake if needed, but we used g++ directly so we'll stick to that
# Update package lists
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy all source files
COPY . .

# Compile the C++ application
# Note: We are compiling efficiently with optimization
RUN g++ main.cpp ChatController.cpp UserManager.cpp MessageManager.cpp -o chatting_app -lpthread -O2

# Expose the port (Render will override this, but good for documentation)
EXPOSE 8080

# Command to run the application
CMD ["./chatting_app"]
